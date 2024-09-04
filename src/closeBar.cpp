//
// Created by beafowl on 9/4/24.
//

#include "closeBar.hpp"
#include <QIcon>
#include <QPixmap>
#include <QToolButton>

QToolButton *createButtonWithStates(const QString &aSpriteSheetPath) {
  QPixmap spriteSheet(aSpriteSheetPath);
  auto *button = new QToolButton();
  const int spriteSize = 114;
  QIcon buttonIcon;

  buttonIcon.addPixmap(
      spriteSheet.copy(0 * spriteSize, 0, spriteSize, spriteSize),
      QIcon::Normal);
  buttonIcon.addPixmap(
      spriteSheet.copy(1 * spriteSize, 0, spriteSize, spriteSize),
      QIcon::Active);
  buttonIcon.addPixmap(
      spriteSheet.copy(2 * spriteSize, 0, spriteSize, spriteSize),
      QIcon::Selected);
  buttonIcon.addPixmap(
      spriteSheet.copy(3 * spriteSize, 0, spriteSize, spriteSize),
      QIcon::Disabled);

  button->setIconSize(QSize(spriteSize, spriteSize));
  button->setIcon(buttonIcon);

  return button;
}

CloseBar::CloseBar(QWidget *aParent, MainWindow *aWindow) : QToolBar(aParent) {
  QToolButton *redButton = new QToolButton();
  QToolButton *yellowButton = new QToolButton();
  QToolButton *greenButton = new QToolButton();
  redButton = createButtonWithStates("./assets/red_buttons.png");
  yellowButton = createButtonWithStates("./assets/yellow_buttons.png");
  greenButton = createButtonWithStates("./assets/green_buttons.png");

  addSeparator();
  addWidget(greenButton);
  addWidget(yellowButton);
  addWidget(redButton);


  this->setToolButtonStyle(Qt::ToolButtonIconOnly);

  setStyleSheet("QToolBar {"
                "  border: 2px solid transparent;"
                "  border-top-left-radius: 0px;"
                "  border-top-right-radius: 0px;"
                "  border-bottom-left-radius: 10px;"
                "  border-bottom-right-radius: 0px;"
                "  background-color: grey ;"
                "  padding: 4px;"
                "}"
                "Separator {"
                "  background-color: black;"
                "  width: 1px;"
                "  margin: 10px;"
                "}");

    if (aWindow) {
        connect(redButton, &QToolButton::clicked, aWindow, &QMainWindow::close);
        connect(yellowButton, &QToolButton::clicked, aWindow, &QMainWindow::showMinimized);
        connect(greenButton, &QToolButton::clicked, aWindow, &QMainWindow::showMaximized);
    }
}

CloseBar::~CloseBar() = default;