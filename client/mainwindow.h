#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QtNetwork/QNetworkReply>
#include <QListWidgetItem>

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
    void onConversationsFetched(QNetworkReply *reply);
    void fetchConversations(int userId);
    void onConversationSelected(QListWidgetItem *item);
    void fetchReceiverName(int userId, const QString &lastMessage);
    void onReceiverNameFetched(QNetworkReply *reply, const QString &lastMessage);
    void setupSideMenu();

private slots:
    void on_loginButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *loginManager;
    QToolBar *toolBar;
    QAction *friendsAction;
    QAction *profileAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *closeAction;
    QSet<int> processedReceiverIds;
};
#endif // MAINWINDOW_H
