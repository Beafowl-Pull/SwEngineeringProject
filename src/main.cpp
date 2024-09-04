//
// Created by beafowl on 9/3/24.
//

#include <QApplication>
#include <QVBoxLayout>
#include <iostream>
#include "customTopBar.hpp"
#include "mainWindow.hpp"
#include "closeBar.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;

    auto *container = new QWidget(&window);
    auto *layout = new QHBoxLayout(container);
    auto *customMenuBar = new CustomTopBar(container);
    auto *closeBar = new CloseBar(container, &window);

    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(customMenuBar);
    layout->addStretch(0);
    layout->addWidget(closeBar);

    window.setCentralWidget(container);
    window.show();

    return QApplication::exec();
}