//
// Created by beafowl on 9/10/24.
//

#ifndef PEERSERVER_HPP
#define PEERSERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

class PeerServer : public QObject {
    Q_OBJECT
public:
    explicit PeerServer(const QString &hostAddress, int port, QObject *parent = nullptr);

    private slots:
        void onNewConnection();
    void onReadyRead();

    private:
    QTcpServer *server;
    QList<QTcpSocket *> clients;
};

#endif //PEERSERVER_HPP
