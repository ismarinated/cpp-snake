/**
 * @file frontend.h
 * @brief Contains declarations of macros and functions for console-based
 * operations.
 */

#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>
#include <string.h>

#include "../../brick_game/snake/controller.h"
#include "../../brick_game/tetris/backend.h"

#define INDEX_W 2

#define GAME_H (HEIGHT + 2)
#define GAME_W (WIDTH * INDEX_W + 2)

#define INFO_W (8 * INDEX_W)
#define CONTROLS_W (8 * INDEX_W)
#define CONTROLS_H 10

WINDOW *create_newwin(int height, int width, int starty, int startx,
                      const char *text, int is_controls);
void init_colors();

int choose_menu_option();
void run_menu(char const **options, int size, WINDOW *win);
void main_loop();

void run_tetris();
void tetris_print_controls(WINDOW *win);
void tetris_print_field(WINDOW *win);
void tetris_print_info(WINDOW *win);
void tetris_get_action(bool *hold);

void run_snake();
void snake_print_controls(WINDOW *win);
void snake_print_field(WINDOW *win);
void snake_print_info(WINDOW *win);
void snake_get_action(bool *hold);

#endif  // FRONTEND_H