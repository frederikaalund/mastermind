#include <iostream>
#include <random>
#include <algorithm>
#include "render.hpp"

using namespace std;

int problem_size = 4;

int play(const vector<int> code) {

    /// We always start with 12 guesses - the zero row is the hidden code.
    int guess_counter = 12;
    int current_guess_index = 0;

    /// The current guess - all initialized to the first color - black.
    vector<int> guess(problem_size);

    char cmd;
    while ('q' != cmd) {

        /// Re-draw the guess and marker after each change.
        for (int k = 0; k < guess.size(); ++k) {
            draw_pegs(guess[k], guess_counter, k);
        }
        draw_col_marker(guess_counter, current_guess_index, problem_size);

        cmd = getchar();
        switch (cmd) {
            case 'w':
                guess[current_guess_index] = (guess[current_guess_index] + 1) % 8;
                break;

            case 's':
                guess[current_guess_index]--;
                if (guess[current_guess_index] < 0) {
                    guess[current_guess_index] = 7;
                }
                break;

            case 'd':
                current_guess_index = (current_guess_index + 1) % problem_size;
                break;

            case 'a':
                current_guess_index--;
                if (current_guess_index < 0) {
                    current_guess_index = problem_size - 1;
                }
                break;

            case ' ':

                /// When 12 guesses have been spent, the player have lost.
                if (1 == guess_counter) {
                    return -1;
                }

                /// Make a copy of the code and guess, which can be changed .
                vector<int> tmp_code = code;
                vector<int> tmp_guess = guess;

                int correct_guess_counter = 0;
                int correct_color_counter = 0;

                /// Count the corect placed gusses. By comparing the guess
                /// to the code - index by index.
                for (int i = 0; i < guess.size(); ++i) {
                    if (tmp_code[i] == tmp_guess[i]) {
                        correct_guess_counter++;
                        tmp_code[i] = -1;
                        tmp_guess[i] = -2;
                    }
                }

                /// Detremine the number of correct colors found, at the incorrect poisiont.
                for (int i = 0; i < guess.size(); ++i) {
                    /// If color not found in the solution - then.
                    if (tmp_code[i] >= 0) {
                        /// Search for a correct color in the set of guesses.
                        for (int j = 0; j < guess.size(); ++j) {
                            /// If the code color is uniq found in the guess:
                            if (tmp_code[i] == tmp_guess[j]) {
                                correct_color_counter++;
                                tmp_code[i] = -1;
                                tmp_guess[j] = -2;
                                break;
                            }
                        }
                    }
                }


                /// Print the feedback to the current guess.
                /// The feedback should not indicate the position of the found feedback. Therefore the
                //// feedback is printed in order of: correct guess and then
                for (int j = 0; j < correct_guess_counter; ++j) {
                    draw_feedback(CORRECT_POS_COLOR, guess_counter, j, problem_size);
                }
                for (int j = correct_guess_counter; j < correct_color_counter + correct_guess_counter; ++j) {
                    draw_feedback(CORRECT_COLOR, guess_counter, j, problem_size);
                }

                /// If all colors was found in the correct position, the game is won.
                if (problem_size == correct_guess_counter) {
                    return 0;
                }

                guess_counter--;
                break;
        }
    }
}

void make_new_code(vector<int> &code) {

    /// Ensure that the old code is removed.
    code.clear();
    /// Create a new code of the problem size
    code.resize(problem_size);

    int current_index = 0;
    char cmd;
    while ('q' != cmd) {

        for (int k = 0; k < code.size(); ++k) {
            draw_pegs(code[k], 0, k);
        }
        draw_col_marker(0, current_index, problem_size);

        cmd = getchar();
        switch (cmd) {
            case 'w':
                code[current_index] = (code[current_index] + 1) % 8;
                break;

            case 's':
                code[current_index]--;
                if (code[current_index] < 0) {
                    code[current_index] = 7;
                }
                break;

            case 'd':
                current_index = (current_index + 1) % problem_size;
                break;

            case 'a':
                current_index--;
                if (current_index < 0) {
                    current_index = problem_size - 1;
                }
                break;

            case ' ':
                return;
        }
    }
}


void make_random_code(vector<int> &code) {

    /// Ensure that the old code is removed.
    code.clear();

    /// Make a new random problem with the available colors.
    /// We do not allow duplicates in this game.
    vector<int> new_code = {0, 1, 2, 3, 4, 5, 6, 7};
    srand(unsigned(time(NULL)));
    random_shuffle(new_code.begin(), new_code.end());

    /// Select only the amount of code needed.
    for (int i = 0; i < problem_size; ++i) {
        code.push_back(new_code[i]);
    }
}

int new_game(vector<int> &code) {

    int player_count = 1;
    char cmd;
    while ('q' != cmd) {

        /// Redraw the menu, showing the current settings.
        draw_menu(problem_size, player_count);

        cmd = getchar();
        switch (cmd) {
            case '1':
                player_count = 1;
                break;
            case '2':
                player_count = 2;
                break;
            case '4':
                problem_size = 4;
                break;
            case '6':
                problem_size = 6;
                break;
            case '8':
                problem_size = 8;
                break;
            case ' ':
                if (1 == player_count) {
                    make_random_code(code);
                } else {
                    /// Clear previous games, before the user select a new code.
                    draw_new_game(problem_size);
                    make_new_code(code);
                }
                return 1;
            case 'q':
                return 0;
        }
    }
}


int main() {

    vector<int> code;

    /// Clear and setup the terminal.
    initialize_terminal();
    draw_new_game(problem_size);

    while (new_game(code)) {

        draw_new_game(problem_size);

        play(code);

        draw_hidden_code(code);
    }

    reset_terminal();

    return 0;
}