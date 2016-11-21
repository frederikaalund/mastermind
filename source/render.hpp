
#ifndef MASTERMIND_RENDER_H
#define MASTERMIND_RENDER_H

#include <string>
#include <vector>

void set_font_color(int color);

void set_background_color(int color);

void clear_screen();

void set_cursor(int row, int col);

void hide_cursor();

void show_cursor();

void draw_square(int row, int col, int height, int width, int color);

void draw_ascii_art(std::vector<std::string> art, int row, int col);

void draw_pegs(int color, int row, int col);

void draw_feedback(int color, int row, int col, int problem_size);

void draw_col_marker(int row, int col, int problem_size);

void draw_new_game(int problem_size);

void draw_menu(int level, int player_count);

void draw_hidden_code(std::vector<int> code);

void initialize_terminal();

void reset_terminal();

/*
 * Mastermind specific output settings.
 */

#define  BLACK        0
#define  RED          1
#define  GREEN        2
#define  YELLOW       3
#define  BLUE         4
#define  PURPLE       5
#define  CYAN         6
#define  WHITE        7
#define  GREY         8


/// Game colors
#define  MARKER                 239
#define  BOARD_COLOR            242
#define  UNMARKED_COLOR         245
#define  HIDDEN_CODE_COLOR       68

#define  CORRECT_POS_COLOR        1
#define  CORRECT_COLOR            7

/// Game dimmensions
const int board_height    = 55;
const int board_width     = 125;

const int menu_row_margin = 18;
const int menu_col_margin = 80;

const int game_row_margin = 3;
const int game_col_margin = 5;
const int guess_margin    = 10;
const int game_height     = board_height + game_row_margin;
const int game_width      = board_width + game_col_margin;


#endif //MASTERMIND_RENDER_H
