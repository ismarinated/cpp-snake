#include "tests.h"

TEST(UtilsTest, GetHighScore) { EXPECT_EQ(s21::getHighScore(), 0); }

TEST(UtilsTest, SetHighScore) {
  int score = 5;
  s21::setHighScore(score);

  s21::Snake_GameInfo_t gi =
      s21::Controller::getController().updateCurrentState();
  EXPECT_EQ(gi.high_score, score);
  s21::setHighScore(0);

  s21::freeGame();
}

TEST(UtilsTest, InitState) { EXPECT_EQ(*s21::initState(), -1); }

TEST(UtilsTest, InitAct) { EXPECT_EQ(*s21::initAct(), -1); }

TEST(AppleTest, SingletonInitialization) {
  s21::Apple &apple1 = s21::Apple::getApple();
  s21::Apple &apple2 = s21::Apple::getApple();

  EXPECT_EQ(&apple1, &apple2);
}

TEST(AppleTest, InitApple) {
  s21::Apple &apple = s21::Apple::getApple();

  int x = apple.getAppleX();
  int y = apple.getAppleY();

  EXPECT_GE(x, 1);
  EXPECT_LE(x, WIDTH);
  EXPECT_GE(y, 1);
  EXPECT_LE(y, HEIGHT);
}

TEST(AppleTest, AppleNotOnSnake) {
  s21::Apple &apple = s21::Apple::getApple();
  s21::Snake &snake = s21::Snake::getSnake();

  bool onSnake = false;
  for (const auto &unit : snake.getUnits()) {
    if (unit.x == apple.getAppleX() && unit.y == apple.getAppleY()) {
      onSnake = true;
      break;
    }
  }
  EXPECT_FALSE(onSnake);
}

TEST(AppleTest, RespawnChangesPosition) {
  s21::Apple &apple = s21::Apple::getApple();

  int oldX = apple.getAppleX();
  int oldY = apple.getAppleY();

  apple.spawnApple();

  int newX = apple.getAppleX();
  int newY = apple.getAppleY();

  EXPECT_FALSE(oldX == newX && oldY == newY);
}

TEST(SnakeTest, SingletonInitialization) {
  s21::Snake &snake1 = s21::Snake::getSnake();
  s21::Snake &snake2 = s21::Snake::getSnake();

  EXPECT_EQ(&snake1, &snake2);
}

TEST(SnakeTest, InitSnake) {
  s21::Snake &snake = s21::Snake::getSnake();

  EXPECT_EQ(snake.getSnakeSize(), SIZE);
}

TEST(SnakeTest, UnitsOrderCheck) {
  s21::Snake &snake = s21::Snake::getSnake();

  auto &units = snake.getUnits();
  int i = 3;
  for (const auto &unit : units) {
    EXPECT_EQ(unit.x, MIDDLE);
    EXPECT_EQ(unit.y, TOP + i);
    i--;
  }
}

TEST(SnakeTest, PushUnit) {
  s21::Snake &snake = s21::Snake::getSnake();
  snake.pushUnit(MIDDLE, 5);

  EXPECT_EQ(snake.getSnakeSize(), SIZE + 1);
}

TEST(SnakeTest, PopUnit) {
  s21::Snake &snake = s21::Snake::getSnake();
  snake.popUnit();

  EXPECT_EQ(snake.getSnakeSize(), SIZE);
}

TEST(ControllerTest, SingletonInitialization) {
  s21::Controller &ctrl1 = s21::Controller::getController();
  s21::Controller &ctrl2 = s21::Controller::getController();

  EXPECT_EQ(&ctrl1, &ctrl2);
}

TEST(ControllerTest, GameInitialization) {
  s21::Controller &ctrl = s21::Controller::getController();
  s21::Snake_GameInfo_t gi = ctrl.updateCurrentState();

  EXPECT_EQ(gi.score, 0);
  EXPECT_EQ(gi.high_score, s21::getHighScore());
  EXPECT_EQ(gi.level, 0);
  EXPECT_EQ(gi.speed, GET_SPEED(0));

  int filled_units = 0;
  for (int i = 0; i < M_HEIGHT; i++) {
    for (int j = 0; j < M_WIDTH; j++) {
      if (gi.field[i][j] == 5) filled_units++;
    }
  }
  EXPECT_EQ(filled_units, 0);

  s21::freeGame();
}

TEST(ControllerTest, GameStart) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;

  ctrl.userInput(StartGame, false);
  EXPECT_EQ(*s21::initState(), Shifting);

  s21::Snake_GameInfo_t gi = ctrl.updateCurrentState();

  EXPECT_EQ(gi.score, 0);
  EXPECT_EQ(gi.high_score, s21::getHighScore());
  EXPECT_EQ(gi.level, 0);
  EXPECT_EQ(gi.speed, GET_SPEED(0));

  int filled_units = 0;
  for (int i = 0; i < M_HEIGHT; i++) {
    for (int j = 0; j < M_WIDTH; j++) {
      if (gi.field[i][j] == 5) filled_units++;
    }
  }
  EXPECT_GT(filled_units, 0);

  s21::freeGame();
}

TEST(ControllerTest, MoveSnake) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);

  s21::Snake &snake = s21::Snake::getSnake();
  auto &units = snake.getUnits();

  int prevX = units.front().x;
  int prevY = units.front().y;

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  int curX = units.front().x;
  int curY = units.front().y;

  EXPECT_TRUE(prevX == curX && (curY - prevY == 1));

  s21::freeGame();
}

TEST(ControllerTest, EatApple) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);

  s21::Apple &apple = s21::Apple::getApple();
  int newUnitX = apple.getAppleX();
  int newUnitY = apple.getAppleY();

  bool flag = true;
  while (flag) {
    flag = false;

    if (newUnitY == 1) {
      apple.spawnApple();
      flag = true;
    } else {
      newUnitX = apple.getAppleX();
      newUnitY = apple.getAppleY();
    }
  }

  s21::Snake &snake = s21::Snake::getSnake();
  snake.pushUnit(newUnitX, newUnitY - 1);

  int prevSize = snake.getSnakeSize();
  int prevScore = ctrl.updateCurrentState().score;

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  int curSize = snake.getSnakeSize();
  int curScore = ctrl.updateCurrentState().score;

  EXPECT_TRUE(curSize - prevSize == 1);
  EXPECT_TRUE(curScore - prevScore == 1);

  s21::freeGame();
}

TEST(ControllerTest, ShiftLeft) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);
  ctrl.userInput((UserAction_t)-1, false);

  s21::Snake &snake = s21::Snake::getSnake();
  auto &units = snake.getUnits();

  int prevX = units.front().x;
  int prevY = units.front().y;

  ctrl.userInput(LeftArrow, false);

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  int curX = units.front().x;
  int curY = units.front().y;

  EXPECT_TRUE((curX - prevX == 1) && curY == prevY);

  s21::freeGame();
}

TEST(ControllerTest, ShiftRight) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);
  ctrl.userInput((UserAction_t)-1, false);

  s21::Snake &snake = s21::Snake::getSnake();
  auto &units = snake.getUnits();

  int prevX = units.front().x;
  int prevY = units.front().y;

  ctrl.userInput(RightArrow, false);

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  int curX = units.front().x;
  int curY = units.front().y;

  EXPECT_TRUE(prevX == curX && (curY - prevY == 1));

  s21::freeGame();
}

TEST(ControllerTest, Pause) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);
  ctrl.userInput(Pause, false);

  s21::Snake &snake = s21::Snake::getSnake();
  auto &units = snake.getUnits();

  int prevX = units.front().x;
  int prevY = units.front().y;

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  int curX = units.front().x;
  int curY = units.front().y;

  EXPECT_TRUE(prevX == curX && curY == prevY);

  s21::freeGame();
}

TEST(ControllerTest, GameOver) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);
  ctrl.userInput(EndGame, false);

  EXPECT_TRUE(*s21::initState() == GameOver);

  s21::freeGame();
}

TEST(ControllerTest, CheckCollision) {
  s21::Controller &ctrl = s21::Controller::getController();

  *s21::initState() = Start;
  ctrl.userInput(StartGame, false);

  s21::Snake &snake = s21::Snake::getSnake();
  snake.pushUnit(MIDDLE, 20);

  s21::Game &game = s21::Game::getGame();
  game.setStartTime(std::chrono::steady_clock::now() -
                    std::chrono::milliseconds(1000));

  ctrl.userInput((UserAction_t)-1, false);

  EXPECT_TRUE(*s21::initState() == GameOver);

  s21::freeGame();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}