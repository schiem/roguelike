/**
 *  MAIN_MENU.CPP
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main_menu.h"
using namespace std;

const string MainMenu::title="POOP BUTTS: The Reckoning";

MainMenu::MainMenu(int w, int h) {
    STARTING_WIDTH = w;
    STARTING_HEIGHT = h;

    selection = 0;
    done_selecting = false;
    options = std::vector<std::string>(2);
    options[0] = "New Game";
    options[1] = "Continue Saved Game";
}

bool MainMenu::out_of_bounds(int row) {
    return ((row < 0) || (row >= options.size()));
}

/**
 * Changes the selection menu option.
 */
void MainMenu::move_selection(int direction) {
    int new_selection = selection + direction;
    //That rhymed.
    if(!out_of_bounds(new_selection)) {
        selection = new_selection;
    }
}

void MainMenu::make_selection() {
    done_selecting = true;
}

bool MainMenu::is_done_selecting() {
    return done_selecting;
}

void MainMenu::render(SDL_Surface* ascii, SDL_Surface* screen) {
    int starting_col;
    starting_col = (STARTING_WIDTH - title.size()) / 2;
    drawStr(starting_col, (STARTING_HEIGHT/4), title.c_str(),
            ascii, screen, RED);

    int color, string_size;
    //Render selections
    for(int i = 0; i < options.size(); i++) {
        string_size = options[i].size();
        starting_col = (STARTING_WIDTH - string_size) / 2;
        
        if(selection == i) {
            color = WHITE;
        } else {
            color = DARK_GRAY;
        }

        drawStr(starting_col, (STARTING_HEIGHT/2) + i,
                options[i].c_str(), ascii, screen, color);
    }

}

int MainMenu::get_selection() {
    return selection;
}
