#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QtNetwork/QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);
    void updateToolbarButtons();
    void onLoginResponse(QNetworkReply *reply);

private slots:
    void on_loginButton_clicked();

private:
    Ui::MainWindow *ui;
    QToolBar *toolBar;
    QAction *friendsAction;
    QAction *profileAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *closeAction;
};
#endif // MAINWINDOW_H
