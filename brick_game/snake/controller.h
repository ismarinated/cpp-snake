/**
 * @file controller.h
 * @brief Contains s21::Controller class and its methods.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

namespace s21 {

/// Manages input and output operations for the frontend.
class Controller {
 private:
  Controller() = default;

 public:
  static Controller& getController() {
    static Controller controller;
    return controller;
  }

  void userInput(UserAction_t action, bool hold) {
    Game::getGame().handleState(action, hold);
  }

  Snake_GameInfo_t updateCurrentState() {
    Game::getGame();
    Snake_GameInfo_t* gi = initGameInfo();
    return *gi;
  }

  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

  ~Controller() = default;
};

};  // namespace s21

#endif  // CONTROLLER_H