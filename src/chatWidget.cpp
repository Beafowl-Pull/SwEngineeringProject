//
// Created by beafowl on 9/10/24.
//

#include <QMessageBox>
#include "chatWidget.hpp"


chatWidget::chatWidget(const QString &hostAddress, int port, QWidget *parent)
    : QWidget(parent), client(new PeerClient(this)) {

    client->connectToPeer(hostAddress, port);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    auto *inputLayout = new QHBoxLayout;
    auto *mainLayout = new QVBoxLayout(this);

    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    mainLayout->addLayout(inputLayout);

    connect(sendButton, &QPushButton::clicked, this, &chatWidget::onSendMessage);
}

void chatWidget::onSendMessage() {
    QString message = messageInput->text().trimmed();

    if (!message.isEmpty()) {
        if (client->isConnected()) {
            client->sendMessage(message);
            messageInput->clear();
        } else {
            QMessageBox::warning(this, "Connection Error", "The connection is not open.");
        }
    }
}