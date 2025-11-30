/**
 * @file frontend.c
 * @brief Handles the visual part in console.
 */

#include "frontend.h"

WINDOW *create_newwin(int height, int width, int starty, int startx,
                      const char *text, int is_controls) {
  WINDOW *win = newwin(height, width, starty, startx);
  box(win, 0, 0);
  mvwprintw(win, 0, (width - strlen(text)) / 2, "%s", text);
  wrefresh(win);

  if (is_controls == 1) tetris_print_controls(win);
  if (is_controls == 0) snake_print_controls(win);

  return win;
}

void init_colors() {
  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(2, COLOR_CYAN, COLOR_CYAN);
  init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(4, COLOR_WHITE, COLOR_WHITE);
  init_pair(5, COLOR_GREEN, COLOR_GREEN);
  init_pair(6, COLOR_BLUE, COLOR_BLUE);
  init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
}

int choose_menu_option() {
  int flag = 0;
  char ch = wgetch(stdscr);

  switch (ch) {
    case (char)KEY_DOWN:
      flag = 1;
      break;
    case (char)KEY_UP:
      flag = 3 - 1;
      break;
    case '\n':
      flag = -1;
      break;
  }

  return flag;
}

void run_menu(char const **options, int size, WINDOW *win) {
  int flag = 0;
  int highlight = 0;

  while (flag != -1) {
    for (int i = 0; i < size; i++) {
      if (i == highlight) wattron(win, A_REVERSE);
      mvwprintw(win, i + 1, 1, "%s", options[i]);

      wattroff(win, A_REVERSE);
    }
    wrefresh(win);

    int option = choose_menu_option();
    if (option != -1)
      highlight = (highlight + option) % 3;
    else {
      delwin(win);

      if (highlight == 0)
        run_tetris();
      else if (highlight == 1)
        run_snake();

      flag = -1;
    }
  }
}

void main_loop() {
  WINDOW *menu = create_newwin(5, 10, 0, 0, "MENU", -1);
  start_color();
  init_colors();

  char const *options[] = {"Tetris", "Snake", "Exit"};
  int size = sizeof(options) / sizeof(options[0]);

  run_menu(options, size, menu);
}

void run_tetris() {
  WINDOW *field = create_newwin(GAME_H, GAME_W, 0, CONTROLS_W, "TETRIS", -1);
  WINDOW *controls = create_newwin(CONTROLS_H, CONTROLS_W, 0, 0, "CONTROLS", 1);
  WINDOW *info =
      create_newwin(GAME_H, INFO_W, 0, CONTROLS_W + GAME_W, "INFO", -1);

  UserAction_t *act = init_act();
  struct timespec start;
  clock_gettime(CLOCK_MONOTONIC, &start);

  while ((int)*init_state() != GameOver) {
    bool hold = false;

    tetris_get_action(&hold);
    userInput(*act, hold);
    moving(&start);
    *act = (UserAction_t)-1;

    tetris_print_field(field);
    tetris_print_info(info);
  }

  free_game();
  set_high_score(init_game_info()->high_score);

  delwin(field);
  delwin(controls);
  delwin(info);
}

void tetris_print_controls(WINDOW *win) {
  int i = 1;

  mvwprintw(win, i++, 1, "Enter - start");
  mvwprintw(win, i++, 1, "q - quit");
  mvwprintw(win, i++, 1, "p - pause");
  i++;
  mvwprintw(win, i++, 1, "< - left");
  mvwprintw(win, i++, 1, "> - right");
  mvwprintw(win, i++, 1, "v - drop");
  mvwprintw(win, i++, 1, "r - rotate");

  wrefresh(win);
}

void tetris_print_field(WINDOW *win) {
  GameInfo_t gi = updateCurrentState();
  Figure_t *fig = init_figure();

  for (int i = 1; i <= HEIGHT; i++) {
    for (int j = 1; j <= WIDTH; j++) {
      if (!gi.field[i][j])
        mvwprintw(win, i, j * INDEX_W - 1, "..");
      else {
        wattron(win, COLOR_PAIR(gi.field[i][j]));
        mvwprintw(win, i, j * INDEX_W - 1, "##");
        wattroff(win, COLOR_PAIR(gi.field[i][j]));
      }
    }
  }

  if (fig->y != 1) {
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
        if (fig->cur[i][j]) {
          wattron(win, COLOR_PAIR(fig->type));
          mvwprintw(win, fig->y + i - 1, (fig->x + j - 1) * INDEX_W - 1, "##");
          wattroff(win, COLOR_PAIR(fig->type));
        }
      }
    }
  }

  wrefresh(win);
}

void tetris_print_info(WINDOW *win) {
  GameInfo_t gi = updateCurrentState();

  if ((int)*init_state() != -1 && (int)*init_state() != GameOver) {
    int i = 1;
    mvwprintw(win, i++, 1, "High Score:");
    mvwprintw(win, i++, 3, "%d", gi.high_score);
    mvwprintw(win, i++, 1, "Score:");
    mvwprintw(win, i++, 3, "%d", gi.score);
    mvwprintw(win, i++, 1, "Level:");
    mvwprintw(win, i++, 3, "%d", gi.level);
    mvwprintw(win, i++, 1, "Next Figure:");

    for (int n = 0; n < SIZE; n++) {
      for (int m = 0; m < SIZE; m++) {
        if (gi.next[n][m] != 0) {
          wattron(win, COLOR_PAIR(init_figure()->next_type));
          mvwprintw(win, i + n, 3 + (m + 1) * INDEX_W - 1, "##");
          wattroff(win, COLOR_PAIR(init_figure()->next_type));
        } else
          mvwprintw(win, i + n, 3 + (m + 1) * INDEX_W - 1, "..");
      }
    }
  }

  wrefresh(win);
}

void tetris_get_action(bool *hold) {
  char ch = wgetch(stdscr);
  UserAction_t *act = init_act();
  State_t *state = init_state();

  switch (ch) {
    case '\n':
      *act = StartGame;
      if ((int)*state == -1) *state = Start;
      break;
    case 'q':
      *act = EndGame;
      break;
    case 'p':
      *act = Pause;
      break;
    case (char)KEY_LEFT:
      *act = LeftArrow;
      break;
    case (char)KEY_RIGHT:
      *act = RightArrow;
      break;
    case (char)KEY_DOWN:
      *act = DownArrow;
      *hold = true;
      break;
    case 'r':
      *act = Action;
      break;
  }
}

void run_snake() {
  WINDOW *field = create_newwin(GAME_H, GAME_W, 0, CONTROLS_W, "SNAKE", -1);
  WINDOW *controls =
      create_newwin(CONTROLS_H - 1, CONTROLS_W, 0, 0, "CONTROLS", 0);
  WINDOW *info =
      create_newwin(GAME_H, INFO_W, 0, CONTROLS_W + GAME_W, "INFO", -1);

  UserAction_t *act = s21::initAct();

  while ((int)*s21::initState() != GameOver) {
    bool hold = false;

    snake_get_action(&hold);
    s21::Controller::getController().userInput(*act, hold);
    *act = (UserAction_t)-1;

    snake_print_field(field);
    snake_print_info(info);
  }

  delwin(field);
  delwin(controls);
  delwin(info);
}

void snake_print_controls(WINDOW *win) {
  int i = 1;

  mvwprintw(win, i++, 1, "Enter - start");
  mvwprintw(win, i++, 1, "q - quit");
  mvwprintw(win, i++, 1, "p - pause");
  i++;
  mvwprintw(win, i++, 1, "< - left");
  mvwprintw(win, i++, 1, "> - right");
  mvwprintw(win, i++, 1, "A - speed");

  wrefresh(win);
}

void snake_print_field(WINDOW *win) {
  s21::Snake_GameInfo_t gi =
      s21::Controller::getController().updateCurrentState();

  for (int i = 1; i <= HEIGHT; i++) {
    for (int j = 1; j <= WIDTH; j++) {
      if (!gi.field[i][j])
        mvwprintw(win, i, j * INDEX_W - 1, "..");
      else {
        wattron(win, COLOR_PAIR(gi.field[i][j]));
        mvwprintw(win, i, j * INDEX_W - 1, "##");
        wattroff(win, COLOR_PAIR(gi.field[i][j]));
      }
    }
  }

  wrefresh(win);
}

void snake_print_info(WINDOW *win) {
  s21::Snake_GameInfo_t gi =
      s21::Controller::getController().updateCurrentState();

  int i = 1;
  if ((int)*init_state() != GameOver) {
    mvwprintw(win, i++, 1, "High Score:");
    mvwprintw(win, i++, 3, "%d", gi.high_score);
    mvwprintw(win, i++, 1, "Score:");
    mvwprintw(win, i++, 3, "%d", gi.score);
    mvwprintw(win, i++, 1, "Level:");
    mvwprintw(win, i++, 3, "%d", gi.level);
  }

  wrefresh(win);
}

void snake_get_action(bool *hold) {
  char ch = wgetch(stdscr);
  UserAction_t *act = s21::initAct();
  State_t *state = s21::initState();

  switch (ch) {
    case '\n':
      *act = StartGame;
      if ((int)*state == -1) *state = Start;
      break;
    case 'q':
      *act = EndGame;
      break;
    case 'p':
      *act = Pause;
      break;
    case (char)KEY_LEFT:
      *act = LeftArrow;
      break;
    case (char)KEY_RIGHT:
      *act = RightArrow;
      break;
    case 'a':
      *act = Action;
      *hold = true;
      break;
  }
}

int main() {
  initscr();

  refresh();  // would not work (displ. boxes) w/out

  keypad(stdscr, TRUE);   // using arrows and other spec. keys
  nodelay(stdscr, TRUE);  // for non-blocking input from wgetch()
  cbreak();               // input handling w/out Enter
  curs_set(0);            // hides cursor
  noecho();               // disabling console input

  main_loop();

  endwin();

  return 0;
}