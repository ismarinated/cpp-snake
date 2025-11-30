/**
 * @file snakefield.cc
 * @brief Contains definition for SnakeField class methods.
 */

#include "snakefield.h"

SnakeField::SnakeField(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);

  gameTimer = new QTimer(this);
  inputTimer = new QTimer(this);

  connect(gameTimer, &QTimer::timeout, this, &SnakeField::onGameTick);
  connect(inputTimer, &QTimer::timeout, this, &SnakeField::processInput);

  gameTimer->start(150);
  inputTimer->start(10);
}

void SnakeField::onGameTick() {
  m_gameInfo = s21::Controller::getController().updateCurrentState();
  emit stateUpdated(m_gameInfo);

  if (*s21::initState() == GameOver) {
    emit exitRequested();
  }

  m_hold = false;
  update();
}

void SnakeField::processInput() {
  UserAction_t *act = s21::initAct();
  s21::Controller::getController().userInput(*act, m_hold);
  *act = (UserAction_t)-1;
}

void SnakeField::keyPressEvent(QKeyEvent *event) {
  UserAction_t *act = s21::initAct();
  State_t *state = s21::initState();

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
    case Qt::Key_A:
      *act = Action;
      m_hold = true;
      break;
  }
}

void SnakeField::paintEvent(QPaintEvent *) {
  QPainter painter(this);

  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect());

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (m_gameInfo.field[i + 1][j + 1] == 5)
        painter.setBrush(Qt::green);
      else if (m_gameInfo.field[i + 1][j + 1] == 1)
        painter.setBrush(Qt::red);

      if (m_gameInfo.field[i + 1][j + 1])
        painter.drawRect(j * DOT_WIDTH, i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT);
    }
  }
}
