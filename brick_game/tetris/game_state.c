/**
 * @file game_state.c
 * @brief Handles finite state machine logic.
 */

#include "backend.h"

State_t *init_state() {
  static State_t state = (State_t)-1;
  return &state;
}

UserAction_t *init_act() {
  static UserAction_t act = (UserAction_t)-1;
  return &act;
}

GameInfo_t *init_game_info() {
  static GameInfo_t gi;

  if (!gi.field) {
    gi.field = create_matrix(M_HEIGHT, M_WIDTH);
    gi.high_score = get_high_score();
    gi.level = 0;
    gi.pause = 0;
    gi.score = 0;
    gi.speed = GET_SPEED(gi.level);

    gi.next = create_matrix(SIZE, SIZE);
    Figure_t *fig = init_figure();
    copy_matrix(fig->next, gi.next, SIZE, SIZE);
  }

  return &gi;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *gi = init_game_info();
  return *gi;
}

int is_collision() {
  GameInfo_t *gi = init_game_info();
  Figure_t *fig = init_figure();

  int flag = 0;

  for (int i = 0; i < SIZE && !flag; i++) {
    for (int j = 0; j < SIZE && !flag; j++) {
      if (fig->cur[i][j] && (gi->field[fig->y - 1 + i][fig->x - 1 + j] != 0 ||
                             fig->y - 1 + i > HEIGHT || fig->x - 1 + j < 1 ||
                             fig->x - 1 + j > WIDTH))
        flag = 1;
    }
  }

  return flag;
}

void userInput(UserAction_t action, bool hold) {
  State_t *state = init_state();
  GameInfo_t *gi = init_game_info();

  if (action == Pause) {
    if (!gi->pause)
      gi->pause = 1;
    else
      gi->pause = 0;
  } else if (action == EndGame)
    *state = GameOver;

  if (!gi->pause && *state == Start) {
    if (action == StartGame) *state = Spawn;
  } else if (!gi->pause && *state == Spawn) {
    if (init_game_info()->level >= MAX_LEVEL)
      *state = GameOver;
    else {
      spawning();
      *state = Moving;
    }
  } else if (!gi->pause && *state == Shifting)
    shifting(hold);
  else if (!gi->pause && *state == Attaching) {
    attaching();
    if (*state != GameOver) {
      scoring();
      *state = Spawn;
    }
  }
}

void scoring() {
  GameInfo_t *gi = init_game_info();

  int flag = 0;
  int rows = 0;
  for (int i = 1; i <= HEIGHT; i++) {
    for (int j = 1; !flag && j <= WIDTH; j++)
      if (!gi->field[i][j]) flag = 1;

    if (!flag) {
      del_row(i);
      rows++;
    }
    flag = 0;
  }

  if (rows >= 1 && rows <= 4) gi->score += GET_SCORE(rows);
  if (gi->high_score < gi->score) gi->high_score = gi->score;
  gi->level = gi->score / 600;
  gi->speed = GET_SPEED(gi->level);
}

void del_row(int row) {
  GameInfo_t *gi = init_game_info();

  for (int i = HEIGHT; i >= 1; i--) {
    for (int j = 1; j <= WIDTH; j++)
      if (i <= row) gi->field[i][j] = gi->field[i - 1][j];
  }
}

void moving(struct timespec *start) {
  Figure_t *fig = init_figure();
  State_t *state = init_state();

  if (*state == Moving) {
    long speed_ms = (long)init_game_info()->speed;

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long elapsed_ms = (now.tv_sec - start->tv_sec) * 1000L +
                      (now.tv_nsec - start->tv_nsec) / 1000000L;

    if (elapsed_ms >= speed_ms) {
      *start = now;

      fig->y++;
      if (is_collision()) {
        fig->y--;
        *state = Attaching;
      }
    } else
      *state = Shifting;
  }
}

void spawning() {
  Figure_t *fig = init_figure();

  copy_matrix(fig->next, fig->cur, SIZE, SIZE);
  fig->type = fig->next_type;
  set_to_start();

  fill_figure();

  GameInfo_t *gi = init_game_info();
  copy_matrix(fig->next, gi->next, SIZE, SIZE);
}

void shifting(bool hold) {
  Figure_t *fig = init_figure();
  State_t *state = init_state();
  UserAction_t *act = init_act();

  if (*act == RightArrow)
    move_right();
  else if (*act == LeftArrow)
    move_left();
  else if (*act == DownArrow && hold)
    move_down();
  else if (*act == Action) {
    rotate(fig->cur);
    if (is_collision()) {
      if (fig->type != 1)
        fig->x--;
      else
        fig->x -= 3;
    }
  } else
    *state = Moving;
}

void attaching() {
  GameInfo_t *gi = init_game_info();
  Figure_t *fig = init_figure();

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (fig->cur[i][j] && fig->y + i - 1 != TOP)
        gi->field[fig->y + i - 1][fig->x + j - 1] = fig->cur[i][j];
      else if (fig->cur[i][j] && fig->y + i - 1 == TOP)
        *init_state() = GameOver;
    }
  }
}

void free_game() {
  GameInfo_t *gi = init_game_info();

  free_matrix(gi->field, M_HEIGHT);
  free_matrix(gi->next, SIZE);

  Figure_t *fig = init_figure();

  free_matrix(fig->cur, SIZE);
  free_matrix(fig->next, SIZE);
}