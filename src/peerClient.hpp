//
// Created by beafowl on 9/10/24.
//

#ifndef PEERCLIENT_HPP
#define PEERCLIENT_HPP

#include <QTcpSocket>
#include <QObject>
#include <QDebug>

class PeerClient : public QObject {
    Q_OBJECT
public:
    explicit PeerClient(QObject *parent = nullptr);
    void connectToPeer(const QString &address, int port);
    void sendMessage(const QString &message);
    void disconnectFromHost();
    bool isConnected() const;

    signals:
        void messageReceived(const QString &message);

    private slots:
        void onReadyRead();

    private:
        QTcpSocket *socket;
};

#endif //PEERCLIENT_HPP
