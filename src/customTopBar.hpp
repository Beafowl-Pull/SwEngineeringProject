//
// Created by beafowl on 9/4/24.
//

#ifndef CUSTOMTOPBAR_HPP
#define CUSTOMTOPBAR_HPP

#include <QWidget>

class CustomTopBar : public QWidget {
  Q_OBJECT

public:
  explicit CustomTopBar(QWidget *aParent = nullptr);
  ~CustomTopBar() override;
};

#endif // CUSTOMTOPBAR_HPP
