//
// Created by beafowl on 9/4/24.
//

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *aParent = nullptr)
        : QMainWindow(aParent) {
    }
    ~MainWindow() = default;
};

#endif // MAINWINDOW_HPP
