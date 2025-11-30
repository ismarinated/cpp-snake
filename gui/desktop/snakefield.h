/**
 * @file snakefield.h
 * @brief Contains SnakeField class and its methods declarations.
 */

#ifndef SNAKEFIELD_H
#define SNAKEFIELD_H

#include "view.h"

/// Widget representing the Snake game field in the desktop app.
class SnakeField : public QWidget {
  Q_OBJECT

 public:
  SnakeField(QWidget *parent = nullptr);

 signals:
  void infoUpdated(const s21::Snake_GameInfo_t &gi);
  void exitRequested();
  void stateUpdated(const s21::Snake_GameInfo_t &gi);

 private slots:
  void onGameTick();
  void processInput();

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *) override;

 private:
  static const int DOT_WIDTH = 20;
  static const int DOT_HEIGHT = 20;
  static const int FIELD_WIDTH = 10;
  static const int FIELD_HEIGHT = 20;

  static const int DELAY = 100;

  QTimer *gameTimer;
  QTimer *inputTimer;

  bool m_hold = false;
  s21::Snake_GameInfo_t m_gameInfo;
};

#endif  // SNAKEFIELD_H
