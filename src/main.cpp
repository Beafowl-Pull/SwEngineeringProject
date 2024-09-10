//
// Created by beafowl on 9/3/24.
//

#include <QApplication>
#include <iostream>
#include "peerServer.hpp"
#include "closeBar.hpp"
#include "customTopBar.hpp"
#include "mainWindow.hpp"
#include "chatWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
        return -1;
    }

    QString hostAddress = argv[1];
    int port = QString(argv[2]).toInt();

    if (port < 1024 || port > 65535) {
        std::cerr << "Invalid port number. Please use a port number between 1024 and 65535." << std::endl;
        return -1;
    }

    PeerServer server = PeerServer(hostAddress, port);

    window.show();

    return QApplication::exec();
}
