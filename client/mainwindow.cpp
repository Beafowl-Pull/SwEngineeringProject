#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>
#include <QListWidget>
#include <qjsonarray.h>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);

    toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->setFixedHeight(30);
    toolBar->setStyleSheet("QToolBar { background-color: rgba(0, 0, 0, 0.4); border: none; }");

    friendsAction = new QAction("Friends", this);
    profileAction = new QAction("Profile", this);
    toolBar->addAction(friendsAction);
    toolBar->addAction(profileAction);

    friendsAction->setVisible(false);
    profileAction->setVisible(false);

    minimizeAction = new QAction(QIcon(":/assets/minimize.png"), "Minimize", this);
    maximizeAction = new QAction(QIcon(":/assets/maximize.png"), "Maximize", this);
    closeAction = new QAction(QIcon(":/assets/close.png"), "Close", this);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);

    toolBar->addAction(minimizeAction);
    toolBar->addAction(maximizeAction);
    toolBar->addAction(closeAction);

    connect(minimizeAction, &QAction::triggered, this, &QWidget::showMinimized);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    setupConversationPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateToolbarButtons()
{
    toolBar->removeAction(minimizeAction);
    toolBar->removeAction(maximizeAction);

    if (isMaximized()) {
        toolBar->addAction(minimizeAction);
    } else {
        toolBar->addAction(maximizeAction);
    }

    toolBar->removeAction(closeAction);
    toolBar->addAction(closeAction);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateToolbarButtons();
}

void MainWindow::on_loginButton_clicked()
{
    qDebug() << "clicked";
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8000/login");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = ui->usernameLine->text();
    json["password"] = ui->passwordLine->text();

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    manager->post(request, data);

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onLoginResponse);
}

void MainWindow::onLoginResponse(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Login Failed", "Network error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isObject()) {
        QMessageBox::warning(this, "Login Failed", "Invalid server response.");
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    if (jsonObject.contains("error")) {
        QMessageBox::warning(this, "Login Failed", jsonObject["error"].toString());
    } else {
        int userId = jsonObject["id"].toInt();
        QMessageBox::information(this, "Login Successful", "Welcome, " + jsonObject["username"].toString() + "!");

        fetchConversations(userId);

        ui->stackedWidget->setCurrentIndex(0);
    }

    reply->deleteLater();
}


void MainWindow::onConversationSelected(QListWidgetItem *item)
{
    QString selectedUser = item->text();
    QMessageBox::information(this, "Conversation Selected", "You selected: " + selectedUser);

    // TODO: Load messages for the selected user and display them in the chat area
}

void MainWindow::fetchConversations(int userId)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString url = QString("http://127.0.0.1:8000/conversations/%1").arg(userId);

    QNetworkRequest request((QUrl(url)));

    qDebug() << "Fetching conversations for user:" << userId;

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onConversationsFetched);

    manager->get(request);
}

void MainWindow::onConversationsFetched(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", "Failed to fetch conversations: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isArray()) {
        QMessageBox::warning(this, "Error", "Invalid response format.");
        reply->deleteLater();
        return;
    }

    QJsonArray conversationsArray = jsonDoc.array();
    ui->leftMenu->clear();
    processedReceiverIds.clear();

    for (const QJsonValue &value : conversationsArray) {
        QJsonObject conversation = value.toObject();
        int receiverId = conversation["receiver_id"].toInt();
        QString content = conversation["content"].toString();

        if (!processedReceiverIds.contains(receiverId)) {
            processedReceiverIds.insert(receiverId);
            fetchReceiverName(receiverId, content);
        }
    }

    reply->deleteLater();
}

void MainWindow::fetchReceiverName(int userId, const QString &lastMessage)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString url = QString("http://127.0.0.1:8000/user/%1").arg(userId);

    QNetworkRequest request((QUrl(url)));

    connect(manager, &QNetworkAccessManager::finished, this, [this, lastMessage](QNetworkReply *reply) {
        this->onReceiverNameFetched(reply, lastMessage);
    });

    manager->get(request);
}

void MainWindow::setupConversationPage()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this->centralWidget());

    ui->leftMenu = new QListWidget(this);
    ui->leftMenu->setFixedWidth(200);
    mainLayout->addWidget(ui->leftMenu);

    QWidget *conversationWidget = new QWidget(this);
    QVBoxLayout *conversationLayout = new QVBoxLayout(conversationWidget);

    messageListWidget = new QListWidget(conversationWidget);
    messageListWidget->setStyleSheet(
        "QListWidget::item { border: none; padding: 5px; }"
        "QListWidget::item:selected { background: none; }"
        );
    conversationLayout->addWidget(messageListWidget);

    QWidget *inputBar = new QWidget(conversationWidget);
    QHBoxLayout *inputLayout = new QHBoxLayout(inputBar);

    inputField = new QLineEdit(inputBar);
    inputField->setPlaceholderText("Type your message...");
    QPushButton *sendButton = new QPushButton("Send", inputBar);

    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendButton);

    conversationLayout->addWidget(inputBar);
    mainLayout->addWidget(conversationWidget);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendMessage);
}

void MainWindow::addMessage(const QString &message, bool isSender)
{
    QListWidgetItem *item = new QListWidgetItem(message, messageListWidget);

    if (isSender) {
        item->setTextAlignment(Qt::AlignRight);
        item->setBackgroundColor(Qt::lightGray);
    } else {
        item->setTextAlignment(Qt::AlignLeft);
        item->setBackgroundColor(Qt::white);
    }

    messageListWidget->addItem(item);
    messageListWidget->scrollToBottom(); // Scroll to the bottom for new messages
}

void MainWindow::onSendMessage()
{
    QString message = inputField->text();
    int currentUserId;
    int currentReceiverId;

    if (message.isEmpty()) {
        return;
    }

    addMessage(message, true);
    inputField->clear();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QUrl url("http://127.0.0.1:8000/send_message");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["sender_id"] = currentUserId;
    json["receiver_id"] = currentReceiverId;
    json["content"] = message;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    manager->post(request, data);

    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::onMessageSent);
}

void MainWindow::onMessageSent(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::warning(this, "Error", "Failed to send message: " + reply->errorString());
    }
    reply->deleteLater();
}


void MainWindow::onReceiverNameFetched(QNetworkReply *reply, const QString &lastMessage)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Failed to fetch receiver name: " << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isObject()) {
        qDebug() << "Invalid response format for receiver name.";
        reply->deleteLater();
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    QString receiverName = jsonObject["username"].toString();

    QWidget *itemWidget = new QWidget(ui->leftMenu);
    QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);
    itemLayout->setContentsMargins(5, 5, 5, 5);

    QLabel *receiverLabel = new QLabel(receiverName, itemWidget);
    receiverLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    QLabel *messageLabel = new QLabel(lastMessage, itemWidget);
    messageLabel->setStyleSheet("font-size: 12px; color: gray;");

    itemLayout->addWidget(receiverLabel);
    itemLayout->addWidget(messageLabel);

    itemWidget->setLayout(itemLayout);

    QListWidgetItem *listItem = new QListWidgetItem(ui->leftMenu);
    listItem->setSizeHint(itemWidget->sizeHint());
    ui->leftMenu->addItem(listItem);
    ui->leftMenu->setItemWidget(listItem, itemWidget);

    reply->deleteLater();
}



