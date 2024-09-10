#include "mainWindow.hpp"
#include "chatWidget.hpp"
#include "closeBar.hpp"
#include "customTopBar.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), client(nullptr) {
    auto *centralWidget = new QWidget(this);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    auto *topLayout = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignTop);
    topLayout->setSpacing(0);
    topLayout->setMargin(0);

    customTopBar = new CustomTopBar(this);
    auto *closeBar = new CloseBar(this, this);
    hostNameLabel = new QLabel("Not Connected", this);

    QFont font = hostNameLabel->font();
    font.setPointSize(14);
    font.setBold(true);
    hostNameLabel->setFont(font);
    hostNameLabel->setAlignment(Qt::AlignCenter);

    topLayout->addWidget(customTopBar);
    topLayout->addStretch();
    topLayout->addWidget(hostNameLabel);
    topLayout->addStretch();
    topLayout->addWidget(closeBar);

    mainLayout->addLayout(topLayout);

    messageDisplay = new QTextEdit(this);
    messageDisplay->setReadOnly(true);
    mainLayout->addWidget(messageDisplay);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    sendButton->setEnabled(false);

    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);

    mainLayout->addLayout(inputLayout);

    setCentralWidget(centralWidget);

    connect(customTopBar, &CustomTopBar::connectionMade, this, &MainWindow::onConnectionMade);

    connect(customTopBar, &CustomTopBar::disconnectRequested, this, &MainWindow::onDisconnectRequested);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendMessage);
}

void MainWindow::onConnectionMade(const QString &hostAddress, int port) {
    hostNameLabel->setText("Connected to: " + hostAddress);

    client = new PeerClient(this);
    client->connectToPeer(hostAddress, port);

    if (client->isConnected()) {
        connect(client, &PeerClient::messageReceived, this, &MainWindow::onMessageReceived);

        sendButton->setEnabled(true);
        messageInput->setEnabled(true);

        customTopBar->showDisconnectButton(true);
        qDebug() << "Successfully connected to " << hostAddress;
    } else {
        qDebug() << "Failed to connect to " << hostAddress;
    }
}

void MainWindow::onMessageReceived(const QString &message) {
    messageDisplay->append(message);
}

void MainWindow::onSendMessage() {
    if (client && client->isConnected()) {
        QString message = messageInput->text();
        if (!message.isEmpty()) {
            client->sendMessage(message);
            messageInput->clear();
        }
    } else {
        qDebug() << "Not connected. Unable to send message.";
    }
}

void MainWindow::onDisconnectRequested() {
    if (client && client->isConnected()) {
        client->disconnectFromHost();
        hostNameLabel->setText("Not Connected");
        messageDisplay->clear();
        sendButton->setEnabled(false);
        customTopBar->showDisconnectButton(false);
        delete client;
        client = nullptr;
    }
}

