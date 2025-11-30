/**
 * @file figure.c
 * @brief Handles everything related to four-pixel figure.
 */

#include "backend.h"

Figure_t *init_figure() {
  static Figure_t fig;

  if (!fig.cur) {
    fig.cur = create_matrix(SIZE, SIZE);
    fig.next = create_matrix(SIZE, SIZE);
    fig.type = 0;

    set_to_start();
    fill_figure();
  }

  return &fig;
}

void fill_figure() {
  Figure_t *fig = init_figure();

  zero_matrix(fig->next, SIZE, SIZE);

  switch (fig->next_type) {
    case 1:
      fig->next[0][0] = 1;
      fig->next[0][1] = 1;
      fig->next[0][2] = 1;
      fig->next[0][3] = 1;
      break;
    case 2:
      fig->next[0][0] = 2;
      fig->next[0][1] = 2;
      fig->next[1][0] = 2;
      fig->next[1][1] = 2;
      break;
    case 3:
      fig->next[0][0] = 3;
      fig->next[0][1] = 3;
      fig->next[1][1] = 3;
      fig->next[1][2] = 3;
      break;
    case 4:
      fig->next[0][0] = 4;
      fig->next[1][0] = 4;
      fig->next[2][0] = 4;
      fig->next[2][1] = 4;
      break;
    case 5:
      fig->next[0][0] = 5;
      fig->next[0][1] = 5;
      fig->next[0][2] = 5;
      fig->next[1][1] = 5;
      break;
    case 6:
      fig->next[0][1] = 6;
      fig->next[1][1] = 6;
      fig->next[2][1] = 6;
      fig->next[2][0] = 6;
      break;
    case 7:
      fig->next[0][1] = 7;
      fig->next[0][2] = 7;
      fig->next[1][0] = 7;
      fig->next[1][1] = 7;
      break;
  }

  for (int i = 0; i < fig->next_rot; i++) rotate(fig->next);
}

void set_to_start() {
  Figure_t *fig = init_figure();

  fig->x = MIDDLE;
  fig->y = TOP;

  fig->next_type = randomizer(1, 7);
  fig->next_rot = randomizer(1, 4);
}

void rotate(int **matrix) {
  int **temp = create_matrix(SIZE, SIZE);
  copy_matrix(matrix, temp, SIZE, SIZE);

  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) matrix[i][SIZE - 1 - j] = temp[j][i];
  }

  shift_rows(matrix, SIZE, SIZE);
  shift_columns(matrix, SIZE, SIZE);

  free_matrix(temp, SIZE);
}

void move_right() {
  Figure_t *fig = init_figure();

  fig->x++;
  if (is_collision()) fig->x--;
}

void move_left() {
  Figure_t *fig = init_figure();

  fig->x--;
  if (is_collision()) fig->x++;
}

void move_down() {
  Figure_t *fig = init_figure();
  int flag = 0;

  while (!flag) {
    fig->y++;
    if (is_collision()) {
      fig->y--;
      flag = 1;
    }
  }
}