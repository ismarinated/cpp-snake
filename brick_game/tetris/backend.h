/**
 * @file backend.h
 * @brief Contains macros, structures and functions for Tetris.
 */

#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()

#include "../common/common.h"

#define GET_SCORE(rows)                         \
  ({                                            \
    static int score[] = {100, 300, 700, 1500}; \
    score[rows - 1];                            \
  })

/// Represents the tetris game's state information for the frontend.
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/// Represents the information structure for tetromino handling.
typedef struct {
  int **cur;
  int **next;
  int x;
  int y;
  int type;
  int next_type;
  int next_rot;
} Figure_t;

State_t *init_state();
UserAction_t *init_act();
GameInfo_t *init_game_info();
GameInfo_t updateCurrentState();
int is_collision();
void userInput(UserAction_t action, bool hold);
void scoring();
void del_row(int row);
void moving(struct timespec *start);
void spawning();
void shifting(bool hold);
void attaching();
void free_game();

Figure_t *init_figure();
void fill_figure();
void set_to_start();
void rotate(int **matrix);
void move_right();
void move_left();
void move_down();

int **create_matrix(int n, int m);
void copy_matrix(int **src, int **dest, int n, int m);
void free_matrix(int **matrix, int n);
void zero_matrix(int **matrix, int n, int m);
int randomizer(int min, int max);
void shift_rows(int **matrix, int n, int m);
void shift_columns(int **matrix, int n, int m);
int get_high_score();
void set_high_score(int score);

#endif  // BACKEND_H