/**
 * @file nextfigure.cc
 * @brief Contains definition for NextFigure class methods.
 */

#include "nextfigure.h"

NextFigure::NextFigure(QWidget *parent) : QWidget(parent) {}

void NextFigure::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::NoPen);
  painter.drawRect(rect());
  painter.setPen(Qt::black);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (init_figure()->next_type == 1)
        painter.setBrush(Qt::red);
      else if (init_figure()->next_type == 2)
        painter.setBrush(Qt::cyan);
      else if (init_figure()->next_type == 3)
        painter.setBrush(Qt::yellow);
      else if (init_figure()->next_type == 4)
        painter.setBrush(Qt::gray);
      else if (init_figure()->next_type == 5)
        painter.setBrush(Qt::green);
      else if (init_figure()->next_type == 6)
        painter.setBrush(Qt::blue);
      else if (init_figure()->next_type == 7)
        painter.setBrush(Qt::magenta);

      if (init_figure()->next[i][j])
        painter.drawRect(j * DOT_WIDTH, i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
    }
  }
}
