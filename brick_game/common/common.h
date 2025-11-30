/**
 * @file common.h
 * @brief Contains macros and enums used by both BrickGames.
 */

#ifndef COMMON_H
#define COMMON_H

#define HEIGHT 20
#define WIDTH 10

#define SIZE 4

#define M_HEIGHT (HEIGHT + 2)
#define M_WIDTH (WIDTH + 2)

#define TOP 1
#define MIDDLE (M_WIDTH / 2)

#define MAX_LEVEL 10

#define GET_SPEED(index)                                                     \
  ({                                                                         \
    static int speed[] = {800, 720, 630, 550, 470, 380, 300, 220, 130, 100}; \
    speed[index];                                                            \
  })

// StartGame, Pause, EndGame, LeftArrow, RightArrow, DownArrow, UpArrow, Action
typedef enum {
  StartGame,
  Pause,
  EndGame,
  LeftArrow,
  RightArrow,
  DownArrow,
  UpArrow,
  Action
} UserAction_t;

// Start, Spawn, Moving, Shifting, Attaching, GameOver
typedef enum { Start, Spawn, Moving, Shifting, Attaching, GameOver } State_t;

#endif  // COMMON_H