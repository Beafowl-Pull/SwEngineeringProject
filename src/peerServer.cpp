//
// Created by beafowl on 9/10/24.
//

#include "peerServer.hpp"

#include "peerServer.hpp"

PeerServer::PeerServer(const QString &hostAddress, int port, QObject *parent)
    : QObject(parent) {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &PeerServer::onNewConnection);

    if (!server->listen(QHostAddress(hostAddress), port)) {
        qDebug() << "Server could not start on" << hostAddress << ":" << port;
    } else {
        qDebug() << "Server started on" << hostAddress << ":" << port;
    }
}

void PeerServer::onNewConnection() {
    QTcpSocket *newSocket = server->nextPendingConnection();
    connect(newSocket, &QTcpSocket::readyRead, this, &PeerServer::onReadyRead);
    clients.append(newSocket);  // Keep track of all connected clients
    qDebug() << "New client connected!";
}

void PeerServer::onReadyRead() {
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();
    qDebug() << "Message received:" << data;

    // Send the message to all connected clients except the sender
    for (QTcpSocket *clientSocket : clients) {
        if (clientSocket != senderSocket) {
            clientSocket->write(data);
            clientSocket->flush();
        }
    }
}
