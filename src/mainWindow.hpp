//
// Created by beafowl on 9/4/24.
//

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "peerClient.hpp"
#include "customTopBar.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *aParent = nullptr);
        ~MainWindow() = default;

    private slots:
        void onConnectionMade(const QString &hostAddress, int port);
        void onMessageReceived(const QString &message);
        void onDisconnectRequested();
        void onSendMessage();

    private:
        QLabel *hostNameLabel;
        QTextEdit *messageDisplay;
        QLineEdit *messageInput;
        QPushButton *sendButton;
        PeerClient *client;
        CustomTopBar *customTopBar;
};

#endif // MAINWINDOW_HPP

