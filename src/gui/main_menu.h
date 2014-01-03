/**
 *  MAIN_MENU.H
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

#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <ASCII_Lib.h>

using namespace std;

enum Choice {
    NONE = -1,
    NEW_GAME = 0,
    LOAD_GAME = 1
};

class MainMenu {
    private:
        int selection;
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        bool done_selecting;
        static const string title;
        std::vector<std::string> options;

        bool out_of_bounds(int);

    public:
        MainMenu(int, int);
        void move_selection(int);
        void make_selection();
        bool is_done_selecting();
        //For now, this takes care of its own
        //rendering and knows about SDL. I will likely
        //regret this very soon.
        void render(SDL_Surface*, SDL_Surface*);

        int get_selection();
};

#endif
