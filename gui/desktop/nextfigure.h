/**
 * @file nextfigure.h
 * @brief Contains NextFigure class and its methods declarations.
 */

#ifndef NEXTFIGURE_H
#define NEXTFIGURE_H

#include "view.h"

/// Represents the next tetromino to be displayed in the desktop app.
class NextFigure : public QWidget {
  Q_OBJECT

 public:
  NextFigure(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *) override;

 private:
  static const int DOT_WIDTH = 20;
  static const int DOT_HEIGHT = 20;
};

#endif  // NEXTFIGURE_H
