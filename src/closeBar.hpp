//
// Created by beafowl on 9/4/24.
//

#ifndef CLOSEBAR_HPP
#define CLOSEBAR_HPP

#include <QToolBar>
#include "mainWindow.hpp"

class CloseBar : public QToolBar {
    Q_OBJECT

  public:
    explicit CloseBar(QWidget *aParent = nullptr, MainWindow *aWindow = nullptr);
    ~CloseBar() override;
};

#endif // CLOSEBAR_HPP
