/**
 * @file model.h
 * @brief Contains s21::Snake, s21::Apple, s21::Game classes declarations and
 * definitions of their methods and declarations of additional structures and
 * functions.
 */

#ifndef MODEL_H
#define MODEL_H

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>

#include "../common/common.h"

namespace s21 {

int getHighScore();
void setHighScore(int score);

/// Represents the snake game's state information for the frontend.
typedef struct {
  int **field;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} Snake_GameInfo_t;

Snake_GameInfo_t *initGameInfo();
void freeGame();

State_t *initState();
UserAction_t *initAct();

/// Defines Snake entity and its behavior.
class Snake {
 protected:
  /// Defines Snake's unit structure.
  struct Unit {
    int x;
    int y;
  };

 private:
  std::deque<Unit> units;

  Snake() {
    Unit unit;

    for (int i = 0; i < SIZE; i++) {
      unit.x = MIDDLE;
      unit.y = TOP + i;

      units.push_front(unit);
    }
  };

 public:
  static Snake &getSnake() {
    static Snake snake;
    return snake;
  }

  const std::deque<Unit> &getUnits() const { return units; }

  void pushUnit(int x, int y) { units.push_front(Unit{x, y}); }

  void popUnit() {
    if (!units.empty()) units.pop_back();
  }

  int getSnakeSize() { return units.size(); }

  void reset() {
    units.clear();
    Unit unit;
    for (int i = 0; i < SIZE; i++) {
      unit.x = MIDDLE;
      unit.y = TOP + i;
      units.push_front(unit);
    }
  }

  Snake(const Snake &) = delete;
  Snake &operator=(const Snake &) = delete;

  ~Snake() = default;
};

/// Defines Apple entity and its behavior
class Apple {
 private:
  int x;
  int y;

  static int rnd(int min, int max) {
    return min + (std::rand() % (max - min + 1));
  }

  Apple() { spawnApple(); }

 public:
  static Apple &getApple() {
    static Apple apple;
    return apple;
  }

  void spawnApple() {
    Snake &snake = Snake::getSnake();

    static bool srand_initialized = false;
    if (!srand_initialized) {
      std::srand(static_cast<unsigned int>(std::time(nullptr)));
      srand_initialized = true;
    }

    bool onSnake = true;
    while (onSnake) {
      onSnake = false;
      x = rnd(1, WIDTH);
      y = rnd(1, HEIGHT);

      for (const auto &unit : snake.getUnits()) {
        if (unit.x == x && unit.y == y) {
          onSnake = true;
        }
      }
    }
  }

  int getAppleX() const { return x; }

  int getAppleY() const { return y; }

  Apple(const Apple &) = delete;
  Apple &operator=(const Apple &) = delete;

  ~Apple() = default;
};

/// Defines Game entity and its behavior
class Game {
 private:
  enum Direction { UP, RIGHT, DOWN, LEFT };
  Direction currentDir = DOWN;

  Snake &snake;
  Apple &apple;

  std::chrono::steady_clock::time_point start;

  Game() : snake(Snake::getSnake()), apple(Apple::getApple()) {
    start = std::chrono::steady_clock::now();
  }

 public:
  static Game &getGame() {
    static Game game;
    return game;
  }

  void setStartTime(std::chrono::steady_clock::time_point t) { start = t; }

  void handleState(UserAction_t action, bool hold) {
    State_t *state = initState();
    Snake_GameInfo_t *gi = initGameInfo();

    if (action == EndGame || gi->level >= MAX_LEVEL || gi->score == 200) {
      *state = GameOver;
      setHighScore(gi->high_score);
      freeGame();
    } else if (action == Pause)
      gi->pause = !gi->pause;

    if (!gi->pause && *state == Start) {
      if (action == StartGame) *state = Spawn;
    } else if (!gi->pause && *state == Spawn) {
      *state = Moving;
    } else if (!gi->pause && *state == Shifting)
      shiftSnake(action);

    if ((int)*state != -1 && !gi->pause && *state != GameOver) {
      moveSnake(hold);
      putField();
    }
  }

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  ~Game() = default;

 private:
  bool checkCollision(int newX, int newY) {
    bool flag = false;

    const auto &units = snake.getUnits();

    if (newX < 1 || newX > M_WIDTH - 2 || newY < 1 || newY >= M_HEIGHT - 1)
      flag = true;

    for (size_t i = 1; i < units.size(); i++) {
      if (snake.getSnakeSize() > 4 && units[i].x == newX && units[i].y == newY)
        flag = true;
    }

    return flag;
  }

  void moveSnake(bool hold) {
    Snake_GameInfo_t *gi = initGameInfo();
    int speedMs = gi->speed;
    if (hold) speedMs /= 2;

    auto now = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start)
            .count();

    if (elapsed >= speedMs) {
      int dx = 0;
      int dy = 0;

      switch (currentDir) {
        case UP:
          dy = -1;
          break;
        case DOWN:
          dy = 1;
          break;
        case LEFT:
          dx = -1;
          break;
        case RIGHT:
          dx = 1;
          break;
      }

      const auto &units = snake.getUnits();
      int newX = units.front().x + dx;
      int newY = units.front().y + dy;

      if (checkCollision(newX, newY)) {
        *initState() = GameOver;

        setHighScore(gi->high_score);
        freeGame();
        return;
      }
      if (newX == apple.getAppleX() && newY == apple.getAppleY()) {
        *initState() = Attaching;
        eatApple(newX, newY);
        return;
      }

      snake.pushUnit(newX, newY);
      snake.popUnit();

      setStartTime(now);
    } else
      *initState() = Shifting;
  }

  void eatApple(int newX, int newY) {
    snake.pushUnit(newX, newY);
    apple.spawnApple();

    Snake_GameInfo_t *gi = initGameInfo();

    gi->score = gi->score + 1;
    if (gi->high_score < gi->score) gi->high_score = gi->score;
    gi->level = gi->score / 5;
    gi->speed = GET_SPEED(gi->score / 10);

    *initState() = Moving;
  }

  void shiftSnake(UserAction_t action) {
    switch (action) {
      case LeftArrow:
        currentDir = (Direction)((currentDir + 3) % 4);
        break;
      case RightArrow:
        currentDir = (Direction)((currentDir + 1) % 4);
        break;
      default:
        break;
    }

    *initState() = Moving;
  }

  void putField() {
    Snake_GameInfo_t *gi = initGameInfo();

    for (int i = 0; i < M_HEIGHT; i++) {
      for (int j = 0; j < M_WIDTH; j++) {
        gi->field[i][j] = 0;
      }
    }

    const auto &units = snake.getUnits();
    for (const auto &unit : units) {
      if (unit.y >= 0 && unit.y < M_HEIGHT && unit.x >= 0 && unit.x < M_WIDTH)
        gi->field[unit.y][unit.x] = 5;
    }

    int aX = apple.getAppleX();
    int aY = apple.getAppleY();
    gi->field[aY][aX] = 1;
  }
};

}  // namespace s21

#endif  // MODEL_H