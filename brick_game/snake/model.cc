/**
 * @file model.cc
 * @brief Contains definition for additional Snake functions.
 */

#include "model.h"

namespace s21 {

int getHighScore() {
  int score = 0;

  std::ifstream f("snake_score.txt");
  if (f) f >> score;
  f.close();

  return score;
}

void setHighScore(int score) {
  std::ofstream f("snake_score.txt");
  if (f) f << score;
  f.close();
}

Snake_GameInfo_t *initGameInfo() {
  static Snake_GameInfo_t gi;
  if (!gi.field) {
    gi.field = new int *[M_HEIGHT];
    for (int i = 0; i < M_HEIGHT; i++) gi.field[i] = new int[M_WIDTH]();
    gi.score = 0;
    gi.high_score = getHighScore();
    gi.level = 0;
    gi.pause = 0;
    gi.speed = GET_SPEED(gi.score);
  }
  return &gi;
}

State_t *initState() {
  static State_t state = (State_t)-1;
  return &state;
}

UserAction_t *initAct() {
  static UserAction_t act = (UserAction_t)-1;
  return &act;
}

void freeGame() {
  Snake::getSnake().reset();

  Snake_GameInfo_t *gi = initGameInfo();

  for (int i = 0; i < M_HEIGHT; i++) {
    delete[] gi->field[i];
  }
  delete[] gi->field;
  gi->field = nullptr;
}

};  // namespace s21
