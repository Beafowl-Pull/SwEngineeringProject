//
// Created by beafowl on 9/3/24.
//

#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Main Window");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
