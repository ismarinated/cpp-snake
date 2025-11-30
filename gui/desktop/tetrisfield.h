/**
 * @file tetrisfield.h
 * @brief Contains TetrisField class and its methods declarations.
 */

#ifndef TETRISFIELD_H
#define TETRISFIELD_H

#include "view.h"

/// Widget representing the Tetris game field in the desktop app.
class TetrisField : public QWidget {
  Q_OBJECT

 public:
  TetrisField(QWidget *parent = nullptr);
  ~TetrisField();

 signals:
  void infoUpdated(const GameInfo_t &gi);
  void exitRequested();
  void stateUpdated(const GameInfo_t &gi);

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
  GameInfo_t m_gameInfo;
  struct timespec m_start;
};

#endif  // TETRISFIELD_H
