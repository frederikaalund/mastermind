#include <iostream>
#include <random>
#include <algorithm>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/combine.hpp>
#include "render.hpp"

using namespace std;
namespace adaptors = boost::adaptors;
namespace range = boost::range;
using boost::size;
using boost::get;

template<typename CodePegsIterator>
void wasd_movement(char cmd, code_pegs& code, CodePegsIterator& peg) {
	switch (cmd) {
		case 'w':
			++*peg;
			break;

		case 's':
			--*peg;
			break;

		case 'd':
			peg = next(peg);
			if (code.end() == peg)
				peg = code.begin();
			break;

		case 'a':
			if (code.begin() == peg)
				peg = code.end();
			peg = prev(peg);
			break;
	}
}

void play(const code_pegs& code) {

    /// We always start with 12 guesses - the zero row is the hidden code.
    int guess_counter = 12;

    /// The current guess - all initialized to the first color - black.
    auto guess = code_pegs_presets::all_black(code.size());
	auto peg = guess.begin();

    char cmd;
	do {
        /// Re-draw the guess and marker after each change.
        for (int k = 0; k < guess.size(); ++k) {
            draw_pegs(guess[k], guess_counter, k);
        }
        draw_col_marker(guess_counter, distance(guess.begin(), peg), code.size());

        cmd = (char) getchar();
        switch (cmd) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
				wasd_movement(cmd, guess, peg);
                break;

            case ' ':

                /// When 12 guesses have been spent, the player have lost.
                if (1 == guess_counter) {
                    return;
                }


                /// Make a copy of the code and guess, which can be changed.
                code_pegs tmp_code;
                code_pegs tmp_guess;
				auto unequal = range::combine(code, guess)
					| adaptors::filtered([](auto cg) { return get<0>(cg) != get<1>(cg); });
				for (auto p : unequal) {
					tmp_code.push_back(get<0>(p));
					tmp_guess.push_back(get<1>(p));
				}

				// Number of elements which are equal
				int correct_guess_counter = size(code) - size(tmp_code);

				for (auto p : tmp_code) {
					auto f = range::find(tmp_guess, p);
					if (end(tmp_guess) != f) {
						iter_swap(f, prev(tmp_guess.end()));
						tmp_guess.pop_back();
					}
				}

				int correct_color_counter = size(tmp_code) - size(tmp_guess);

                /// Print the feedback to the current guess. The feedback should not
                /// indicate the position of the found feedback. Therefore the
                /// feedback is printed in order of: correct guess and then
                for (int j = 0; j < correct_guess_counter; ++j) {
                    draw_feedback(CORRECT_POS_COLOR, guess_counter, j, code.size());
                }
                for (int j = correct_guess_counter; j < correct_color_counter + correct_guess_counter; ++j) {
                    draw_feedback(CORRECT_COLOR, guess_counter, j, code.size());
                }

                /// If all colors was found in the correct position, the game is won.
                if (code.size() == correct_guess_counter) {
                    return;
                }

                guess_counter--;
                break;
        } 
    } while ('q' != cmd);
}

void set_new_code(code_pegs& code) {
	assert(!code.empty());

    /// Reset the code to the default black, while resizing.
	code = code_pegs_presets::all_black(code.size());

	auto peg = code.begin();

    char cmd;
	do {
        for (int k = 0; k < code.size(); ++k) {
            draw_pegs(code[k], 0, k);
        }
        draw_col_marker(0, distance(code.begin(), peg), code.size());

        cmd = (char) getchar();
        switch (cmd) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
				wasd_movement(cmd, code, peg);
                break;
            case ' ':
                return;
        }
    } while ('q' != cmd);
}


int setup_new_game(code_pegs& code) {

    int player_count = 1;
    /// Dummy value -  do not use in the switch!
    char cmd = '-';
    while ('q' != cmd) {

        /// Redraw the menu, showing the current settings.
        draw_menu(code.size(), player_count);

        cmd = (char) getchar();
        switch (cmd) {
            case '1':
                player_count = 1;
                break;
            case '2':
                player_count = 2;
                break;
            case '4':
				code.resize(4);
                break;
            case '6':
				code.resize(6);
                break;
            case '8':
				code.resize(8);
                break;
            case ' ':
                if (1 == player_count) {
					code = code_pegs_presets::unique_shuffle(code.size());
                } else {
                    /// Clear previous games, before the user select a new code.
                    draw_new_game(code.size());
                    set_new_code(code);
                }
                return 1;
            case 'q':
                return 0;
        }
    }
    return 0;
}


int main() {
    code_pegs code(4);

    /// Clear and setup the terminal before the game interact with the user.
    initialize_terminal();
    draw_new_game(code.size());

    while (setup_new_game(code)) {

        draw_new_game(code.size());

        play(code);

        show_hidden_code(code);
    }

    reset_terminal();

    return 0;
}
