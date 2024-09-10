//
// Created by beafowl on 9/10/24.
//

#ifndef MESSAGEINPUTWIDGET_HPP
#define MESSAGEINPUTWIDGET_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include "peerClient.hpp"

class chatWidget : public QWidget {
    Q_OBJECT

public:
    explicit chatWidget(const QString &hostAddress, int port, QWidget *parent = nullptr);

    private slots:
        void onSendMessage();

    private:
    QLineEdit *messageInput;
    QPushButton *sendButton;
    PeerClient *client;
};

#endif //MESSAGEINPUTWIDGET_HPP
