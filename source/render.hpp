
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
const int board_height      = 55;
const int board_width       = 125;

/// The margin from the top of the terminal, to the start of the menu area.
const int menu_row_margin   = 18;
/// The margin from the side of the terminal, to the start of the menu area.
const int menu_col_margin   = 80;

/// The margin around the game area.
const int game_row_margin   = 3;
const int game_col_margin   = 5;

/// The margin from the side of the terminal, to the start of the game area.
const int peg_row_margin    = 5;
const int peg_col_margin    = 10;

/// The dimensions of ethe peg.
const int peg_width         = 5;
const int peg_height        = 3;
/// The dimensions of the peg with a border.
const int peg_area_width    = peg_width + 1;
const int peg_area_height   = peg_height + 1;

/// Feedback indicator - white or red indicator of number of correct colors and guesses.
const int feedback_width    = 2;
const int feedback_height   = 1;
/// The dimensions of the peg with a border.
const int feedback_area_width    = feedback_width + 1;
const int feedback_area_height   = feedback_height + 1;

/// Colum marker - which show the current peg.
const int col_marker_width    = peg_width;
const int col_marker_height   = 1;

/// The distance from top to the fist column marker.
const int col_row_margin    = 8;

const int game_height       = board_height + game_row_margin;
const int game_width        = board_width + game_col_margin;



#endif //MASTERMIND_RENDER_H
