#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

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

    minimizeAction = new QAction(QIcon(":/icons/minimize.png"), "Minimize", this);
    maximizeAction = new QAction(QIcon(":/icons/maximize.png"), "Maximize", this);
    closeAction = new QAction(QIcon(":/icons/close.png"), "Close", this);

    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);

    toolBar->addAction(minimizeAction);
    toolBar->addAction(maximizeAction);
    toolBar->addAction(closeAction);

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::on_loginButton_clicked);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::showMinimized);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
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
        QString errorMessage = jsonObject["error"].toString();
        QMessageBox::warning(this, "Login Failed", errorMessage);
    } else {
        QString username = jsonObject["username"].toString();
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");

        friendsAction->setVisible(true);
        profileAction->setVisible(true);
        updateToolbarButtons();
        ui->stackedWidget->setCurrentIndex(0);
    }

    reply->deleteLater();
}
