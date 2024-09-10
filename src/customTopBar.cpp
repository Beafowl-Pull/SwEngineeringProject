//
// Created by beafowl on 9/4/24.
//

#include <QApplication>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>
#include "customTopBar.hpp"
#include "peerClient.hpp"

void CustomTopBar::showConnectDialog() {
    QDialog *dialog = new QDialog(this);

    dialog->setWindowTitle("Connect to Server");

    QLabel *serverLabel = new QLabel("Server Address:", dialog);
    QLineEdit *serverInput = new QLineEdit(dialog);

    QLabel *portLabel = new QLabel("Port:", dialog);
    QLineEdit *portInput = new QLineEdit(dialog);
    portInput->setValidator(new QIntValidator(1024, 65535, dialog)); // Set validator for port input

    QPushButton *connectButton = new QPushButton("Connect", dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", dialog);

    QVBoxLayout *dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->addWidget(serverLabel);
    dialogLayout->addWidget(serverInput);
    dialogLayout->addWidget(portLabel);
    dialogLayout->addWidget(portInput);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(connectButton);
    buttonLayout->addWidget(cancelButton);
    dialogLayout->addLayout(buttonLayout);

    dialog->setLayout(dialogLayout);

    connect(connectButton, &QPushButton::clicked, dialog, [=]() {
        QString serverAddress = serverInput->text();
        QString port = portInput->text();

        if (serverAddress.isEmpty() || port.isEmpty()) {
            QMessageBox::warning(dialog, "Input Error", "Please fill in both fields.");
            return;
        }

        PeerClient *client = new PeerClient();
        client->connectToPeer(serverAddress, port.toInt());

        if (client->isConnected()) {
            emit connectionMade(serverAddress, port.toInt());
            dialog->accept();
        } else {
            QMessageBox::warning(dialog, "Connection Error", "Failed to connect to the server.");
        }
    });

    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

    dialog->exec();
}

void CustomTopBar::showDisconnectButton(bool state) {
    disconnectAction->setVisible(state);
}

CustomTopBar::CustomTopBar(QWidget *aParent) : QWidget(aParent) {
    auto *layout = new QHBoxLayout(this);
    menuBar = new QMenuBar(this);

    QMenu *connectMenu = new QMenu("Menu");

    QAction *connectAction = new QAction("Connect to Server", this);
    connectMenu->addAction(connectAction);

    disconnectAction = new QAction("Disconnect", this);
    disconnectAction->setVisible(false);
    connectMenu->addAction(disconnectAction);

    menuBar->addMenu(connectMenu);
    layout->setMargin(0);
    layout->addWidget(menuBar);
    layout->addStretch(1);
    this->setLayout(layout);

    menuBar->setStyleSheet(
        "QMenuBar {"
        "  border: 2px solid grey;"
        "  border-top-left-radius: 0px;"
        "  border-top-right-radius: 0px;"
        "  border-bottom-left-radius: 0px;"
        "  border-bottom-right-radius: 10px;"
        "  background-color: grey;"
        "}"
        "QMenuBar::item {"
        "  background-color: transparent;"
        "}"
        "QMenuBar::item:selected {"
        "  background-color: transparent;"
        "}"
    );

    connect(connectAction, &QAction::triggered, this, &CustomTopBar::showConnectDialog);

    connect(disconnectAction, &QAction::triggered, this, [=]() {
        emit disconnectRequested();
    });
}
