//
// Created by beafowl on 9/10/24.
//

#include "peerClient.hpp"

PeerClient::PeerClient(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &PeerClient::onReadyRead);
}

void PeerClient::connectToPeer(const QString &address, int port) {
    socket->connectToHost(address, port);
    if (!socket->waitForConnected(3000)) {
        qDebug() << "Connection failed!";
    } else {
        qDebug() << "Connected to peer at" << address << ":" << port;
    }
}

void PeerClient::sendMessage(const QString &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
    } else {
        qDebug() << "Cannot send message: No connection.";
    }
}

bool PeerClient::isConnected() const {
    return socket->state() == QAbstractSocket::ConnectedState;
}

void PeerClient::onReadyRead() {
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);

    emit messageReceived(message);
}

void PeerClient::disconnectFromHost() {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
        if (socket->state() != QAbstractSocket::UnconnectedState) {
            socket->waitForDisconnected(3000);
        }
        qDebug() << "Disconnected from peer";
    } else {
        qDebug() << "No active connection to disconnect.";
    }
}
