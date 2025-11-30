/**
 * @file tetrisfield.cc
 * @brief Contains definition for TetrisField class methods.
 */

#include "tetrisfield.h"

TetrisField::TetrisField(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);

  gameTimer = new QTimer(this);
  inputTimer = new QTimer(this);

  clock_gettime(CLOCK_MONOTONIC, &m_start);

  connect(gameTimer, &QTimer::timeout, this, &TetrisField::onGameTick);
  connect(inputTimer, &QTimer::timeout, this, &TetrisField::processInput);

  gameTimer->start(150);
  inputTimer->start(10);
}

TetrisField::~TetrisField() {
  free_game();
  set_high_score(init_game_info()->high_score);
}

void TetrisField::onGameTick() {
  m_gameInfo = updateCurrentState();
  emit stateUpdated(m_gameInfo);

  if (*init_state() == GameOver) {
    emit exitRequested();
  }

  m_hold = false;
  update();
}

void TetrisField::processInput() {
  UserAction_t *act = init_act();
  userInput(*act, m_hold);
  moving(&m_start);
  *act = (UserAction_t)-1;
}

void TetrisField::keyPressEvent(QKeyEvent *event) {
  UserAction_t *act = init_act();
  State_t *state = init_state();

  int key = event->key();
  switch (key) {
    case Qt::Key_Return:
      *act = StartGame;
      if ((int)*state == -1) *state = Start;
      break;
    case Qt::Key_Q:
      *act = EndGame;
      break;
    case Qt::Key_P:
      *act = Pause;
      break;
    case Qt::Key_Left:
      *act = LeftArrow;
      break;
    case Qt::Key_Right:
      *act = RightArrow;
      break;
    case Qt::Key_Down:
      *act = DownArrow;
      m_hold = true;
      break;
    case Qt::Key_R:
      *act = Action;
      break;
  }
}

void TetrisField::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect());

  Figure_t *fig = init_figure();

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (m_gameInfo.field[i + 1][j + 1] == 1)
        painter.setBrush(Qt::red);
      else if (m_gameInfo.field[i + 1][j + 1] == 2)
        painter.setBrush(Qt::cyan);
      else if (m_gameInfo.field[i + 1][j + 1] == 3)
        painter.setBrush(Qt::yellow);
      else if (m_gameInfo.field[i + 1][j + 1] == 4)
        painter.setBrush(Qt::gray);
      else if (m_gameInfo.field[i + 1][j + 1] == 5)
        painter.setBrush(Qt::green);
      else if (m_gameInfo.field[i + 1][j + 1] == 6)
        painter.setBrush(Qt::blue);
      else if (m_gameInfo.field[i + 1][j + 1] == 7)
        painter.setBrush(Qt::magenta);

      if (m_gameInfo.field[i + 1][j + 1])
        painter.drawRect(j * DOT_WIDTH, i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
    }
  }

  if (fig->y != 1) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        if (fig->type == 1)
          painter.setBrush(Qt::red);
        else if (fig->type == 2)
          painter.setBrush(Qt::cyan);
        else if (fig->type == 3)
          painter.setBrush(Qt::yellow);
        else if (fig->type == 4)
          painter.setBrush(Qt::gray);
        else if (fig->type == 5)
          painter.setBrush(Qt::green);
        else if (fig->type == 6)
          painter.setBrush(Qt::blue);
        else if (fig->type == 7)
          painter.setBrush(Qt::magenta);

        if (fig->cur[i][j])
          painter.drawRect((fig->x + j - 2) * DOT_WIDTH,
                           (fig->y + i - 2) * DOT_HEIGHT, DOT_WIDTH,
                           DOT_HEIGHT);
      }
    }
  }
}
