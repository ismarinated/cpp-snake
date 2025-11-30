/**
 * @file utils.c
 * @brief Handles matrix-related functions, randomizer and text file.
 */

#include "backend.h"

int **create_matrix(int n, int m) {
  int **matrix = (int **)malloc(n * sizeof(int *));

  for (int i = 0; i < n; i++) {
    matrix[i] = (int *)malloc(m * sizeof(int));
  }

  zero_matrix(matrix, n, m);

  return matrix;
}

void copy_matrix(int **src, int **dest, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      dest[i][j] = src[i][j];
    }
  }
}

void free_matrix(int **matrix, int n) {
  for (int i = 0; i < n; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

void zero_matrix(int **matrix, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      matrix[i][j] = 0;
    }
  }
}

int randomizer(int min, int max) {
  static int seed = 0;

  if (!seed) {
    seed = 1;
    srand(time(NULL));
  }

  return rand() % (max - min + 1) + min;
}

void shift_rows(int **matrix, int n, int m) {
  int row = -1;
  int flag = 0;

  for (int i = 0; !flag && i < n; i++) {
    int is_empty = 1;
    for (int j = 0; !flag && j < m; j++) {
      if (matrix[i][j] != 0) {
        is_empty = 0;
        flag = 1;
      }
    }
    if (flag && !is_empty) row = i;
  }

  for (int i = 0; i < n - row; i++) {
    for (int j = 0; j < m; j++) matrix[i][j] = matrix[i + row][j];
  }
  for (int i = n - row; i < n; i++) {
    for (int j = 0; j < m; j++) matrix[i][j] = 0;
  }
}

void shift_columns(int **matrix, int n, int m) {
  int col = -1;
  int flag = 0;

  for (int j = 0; !flag && j < m; j++) {
    int is_empty = 1;
    for (int i = 0; !flag && i < n; i++) {
      if (matrix[i][j] != 0) {
        is_empty = 0;
        flag = 1;
      }
    }
    if (flag && !is_empty) col = j;
  }

  for (int j = 0; j < m - col; j++) {
    for (int i = 0; i < n; i++) matrix[i][j] = matrix[i][j + col];
  }
  for (int j = m - col; j < m; j++) {
    for (int i = 0; i < n; i++) matrix[i][j] = 0;
  }
}

int get_high_score() {
  int score = 0;

  FILE *f = fopen("score.txt", "r");
  if (f) {
    fscanf(f, "%d", &score);
    fclose(f);
  }

  return score;
}

void set_high_score(int score) {
  FILE *f = fopen("score.txt", "w");
  if (f) fprintf(f, "%d", score);
  fclose(f);
}