//
// Created by beafowl on 9/4/24.
//

// #ifndef CUSTOMTOPBAR_HPP
// #define CUSTOMTOPBAR_HPP
//
// #include <QWidget>
//
// class CustomTopBar : public QWidget {
//   Q_OBJECT
//
// public:
//   explicit CustomTopBar(QWidget *aParent = nullptr);
//   void showConnectDialog();
//   ~CustomTopBar() override;
//   signals:
//     void connectionMade(const QString &hostAddress, int port);  // Signal to notify when the connection is established
//
// };
//
//
// #endif // CUSTOMTOPBAR_HPP

#ifndef CUSTOMTOPBAR_HPP
#define CUSTOMTOPBAR_HPP

#include <QWidget>
#include <QMenuBar>

class CustomTopBar : public QWidget {
  Q_OBJECT

    public:
        explicit CustomTopBar(QWidget *parent = nullptr);

    signals:
        void connectionMade(const QString &hostAddress, int port);
        void disconnectRequested();


    public slots:
        void showDisconnectButton(bool state);

    private slots:
        void showConnectDialog();

    private:
        QMenuBar *menuBar;
        QAction *disconnectAction;
};

#endif // CUSTOMTOPBAR_HPP
