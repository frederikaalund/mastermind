#include "render.hpp"

#include <iostream>

/// Control stdio local feedback.
#include <termio.h>
#include <zconf.h>


/*
 * ANSI commands can control the color, cursor position, and much more.
 * The documentation on the ANSI escape commands is found at: https://en.wikipedia.org/wiki/ANSI_escape_code

 * All ANSI commands start with a ASCII ESC char with the value 0x1B followed by a command start '['.
 */

#define ESC 0x1B

std::vector<std::string> game_banner = {
                                        "  __  __           _            __  __ _           _ ",
                                        " |  \\/  |         | |          |  \\/  (_)         | |",
                                        " | \\  / | __ _ ___| |_ ___ _ __| \\  / |_ _ __   __| |",
                                        " | |\\/| |/ _` / __| __/ _ \\ '__| |\\/| | | '_ \\ / _` |",
                                        " | |  | | (_| \\__ \\ ||  __/ |  | |  | | | | | | (_| |",
                                        " |_|  |_|\\__,_|___/\\__\\___|_|  |_|  |_|_|_| |_|\\__,_|"};

using namespace std;

void set_font_color(int color) {
    /// '38;5' is the command to set color from preset ANSI RGB color palette.
    printf("%c[38;5;%dm", ESC, color);
}

void set_background_color(int color) {
    /// 48;5 is the 'extended set background color' command,  the final 'm' is the 'graphic rendition' options.
    printf("%c[48;5;%dm", ESC, color);
}

void clear_screen() {
    /// ANSI color reset command.
    printf("%c[0m", ESC);
    /// ANSI clear screen and cursor command.
    printf("%c[2J", ESC);
}

void set_cursor(int row, int col) {
    printf("%c[%d;%dH", ESC, row, col);
}

void hide_cursor() {
    printf("%c[?25l", ESC);
}

void show_cursor() {
    printf("%c[?25h", ESC);
}

void draw_square(int row, int col, int height, int width, int color) {

    set_background_color(color);
    set_cursor(row, col);

    string row_string((unsigned long) width, ' ');
    for (int i = 0; i < height; ++i) {
        printf("%s", row_string.c_str());
        row++;
        set_cursor(row, col);
    }
}

void draw_ascii_art(vector<string> art, int row, int col) {

    set_font_color(BLACK);
    set_background_color(BOARD_COLOR);
    set_cursor(row, col);
    for (int i = 0; i < art.size(); ++i) {
        printf("%s\n", art[i].c_str());
        row++;
        set_cursor(row, col);
    }
}

void draw_pegs(int color, int row, int col) {

    int target_row = peg_row_margin + row * peg_area_height;
    int target_col = peg_col_margin + col * peg_area_width;

    draw_square(target_row, target_col, peg_height, peg_width, color);
}

void draw_feedback(int color, int row, int col, int problem_size) {

    int target_row = peg_row_margin + row * peg_area_height;

    /// We want two columns of feedback, thus if the col overflow
    /// half the problem size, then set the col back and goto newt feedback row.
    /// Note that coordinates are zero index, while problem size is one index,
    /// - Transform col to 1 indexed, by adding 1 to col.
    if (col + 1 > problem_size / 2) {
        col = col - problem_size / 2;
        target_row += feedback_area_height;
    }

    int target_col = col * feedback_area_width;

    int problem_width = peg_row_margin + (problem_size + 1) * peg_area_width;

    target_col += problem_width;

    draw_square(target_row, target_col, feedback_height, feedback_width, color);
}

void draw_col_marker(int row, int col, int problem_size) {

    int problem_width = peg_row_margin + (problem_size + 1) * peg_area_width;

    int target_row = col_row_margin + row * peg_area_height;
    int target_col = peg_col_margin + col * peg_area_width;

    /// Clear the entire colum marker area for the previous marker row.
    int prev_row = col_row_margin + (row + 1) * peg_area_height;
    /// Only delete if there was a previous marker in the game area.
    if (prev_row < game_height) {
        draw_square(prev_row, peg_col_margin, col_marker_height, problem_width, BOARD_COLOR);
    }

    /// Clear the entire colum marker area - delete the marker.
    draw_square(target_row, peg_col_margin, col_marker_height, problem_width, BOARD_COLOR);
    /// Re-draw the marker at the current position.
    draw_square(target_row, target_col, col_marker_height, col_marker_width, MARKER);
}

void draw_new_game(int problem_size) {
    draw_square(game_row_margin, game_col_margin, board_height, board_width, BOARD_COLOR);

    // TODO: remove Cobham banner before release.
    draw_ascii_art(game_banner, 5, 74);

    /// Draw the top row with the hidden pegs.
    for (int j = 0; j < problem_size; ++j) {
        draw_pegs(HIDDEN_CODE_COLOR, 0, j);
    }

    /// Draw the next 12 rows with unmarked pegs.
    for (int i = 1; i < 13; ++i) {
        for (int j = 0; j < problem_size; ++j) {
            draw_pegs(UNMARKED_COLOR, i, j);
        }
    }

    /// Draw the 12 rows of unmarked feedback indicators.
    for (int i = 1; i < 13; ++i) {
        for (int j = 0; j < problem_size; ++j) {
            draw_feedback(UNMARKED_COLOR, i, j, problem_size);
        }
    }

    /// Draw two rows with the available colors in order.
    for (int j = 0; j < 4; ++j) {
        draw_pegs(j, 10, j + 13);
        draw_pegs(j+4, 11, j + 13);
    }

}

void draw_menu(int level, int player_count) {

    set_font_color(BLACK);
    set_background_color(BOARD_COLOR);

    set_cursor(menu_row_margin + 2, menu_col_margin);
    printf("Difficulty:");
    set_cursor(menu_row_margin + 4, menu_col_margin);
    printf("4 codes - press 4");
    set_cursor(menu_row_margin + 6, menu_col_margin);
    printf("6 codes - press 6");
    set_cursor(menu_row_margin + 8, menu_col_margin);
    printf("8 codes - press 8");

    set_cursor(menu_row_margin + 12, menu_col_margin);
    printf("1 player - press 1\n");
    set_cursor(menu_row_margin + 13, menu_col_margin);
    printf("2 player - press 2\n");

    set_cursor(menu_row_margin + 18, menu_col_margin);
    printf("Start / accept - press space\n");

    set_cursor(menu_row_margin + 20, menu_col_margin);
    printf("Use 'w', 's', 'a', 'd' to set a the pegs.\n");

    set_cursor(menu_row_margin + 22, menu_col_margin);
    printf("Concede / quit - press q\n");


    /// Blank the old markers.
    draw_square(menu_row_margin + 4, menu_col_margin - 2, 10, 1, BOARD_COLOR);

    /// Draw level marker.
    draw_square(menu_row_margin + level, menu_col_margin - 2, 1, 1, BLUE);

    /// Draw player count marker.
    draw_square(menu_row_margin + player_count + 11, menu_col_margin - 2, 1, 1, BLUE);
}

/// Shows the hidden code when game over (lost or won).
void show_hidden_code(vector<int> &code) {
    for (int i = 0; i < code.size(); ++i) {
        draw_pegs(code[i], 0, i);
    }
}

void initialize_terminal() {

    /// Ensure a clean game start.
    clear_screen();

    hide_cursor();

    /// Disable the terminal output.
    termios old_term;
    /// Get the current STDIN settings.
    tcgetattr(STDIN_FILENO, &old_term);

    /// Disable the terminal echo.
    termios new_term = old_term;
    new_term.c_lflag &= ~ECHO;

    /// Get a char on key press - do not wait for 'enter'.
    termios no_block = new_term;
    no_block.c_lflag &= ~ICANON;

    /// Commit the new STD I/O terminal settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &no_block);
}

void reset_terminal() {
    show_cursor();

    /// Disable the terminal output.
    termios old_term;
    /// Get the current STDIN settings.
    tcgetattr(STDIN_FILENO, &old_term);

    /// Enable the terminal echo.
    termios new_term = old_term;
    new_term.c_lflag |= ECHO;

    /// Get a char on 'enter' press.
    termios no_block = new_term;
    no_block.c_lflag |= ICANON;

    /// Commit the new STD I/O terminal settings.
    tcsetattr(STDIN_FILENO, TCSANOW, &no_block);
}