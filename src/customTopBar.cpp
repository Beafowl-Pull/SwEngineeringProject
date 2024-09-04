//
// Created by beafowl on 9/4/24.
//

#include <QApplication>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QWidget>
#include <iostream>
#include "customTopBar.hpp"

CustomTopBar::CustomTopBar(QWidget *aParent) : QWidget(aParent) {
    auto *layout = new QHBoxLayout(this);
    auto *menuBar = new QMenuBar(this);

    // Create QMenu objects
    QMenu *z = new QMenu("Z");
    QMenu *fileMenu = new QMenu("File");
    QMenu *editMenu = new QMenu("Edit");
    QMenu *viewMenu = new QMenu("View");

    // Add QMenu objects to the QMenuBar
    menuBar->addMenu(z);
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(viewMenu);

    layout->setMargin(0);

    // Add actions to the QMenu objects
    fileMenu->addAction("New");
    fileMenu->addAction("Open");
    fileMenu->addAction("Close");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save All");
    fileMenu->addAction("Settings");

    // Apply styles here
    menuBar->setStyleSheet(
        "QMenuBar {"
        "  border: 2px solid grey;"
        "  border-top-left-radius: 0px;"
        "  border-top-right-radius: 0px;"
        "  border-bottom-left-radius: 0px;"
        "  border-bottom-right-radius: 10px;"
        "  background-color: grey;"
        "}"
        "QMenuBar::item {"
        "  background-color: transparent;"
        "}"
        "QMenuBar::item:selected {"
        "  background-color: transparent;"
        "}"
    );

    layout->addWidget(menuBar);
    layout->addStretch(1);

    this->setLayout(layout);
}

CustomTopBar::~CustomTopBar() = default;
