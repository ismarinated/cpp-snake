/**
 * @file view.h
 * @brief Contains View class and its methods declarations.
 */

#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QKeyEvent>
#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QPainter>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/controller.h"
#include "../../brick_game/tetris/adapter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

/// Widget representing the main window in the desktop app.
class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 private slots:
  void on_listWidget_itemClicked(QListWidgetItem *item);

 private:
  Ui::View *ui;
};

#endif  // VIEW_H
