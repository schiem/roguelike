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

#ifndef _MENU_H
#define _MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <ASCII_Lib.h>
#include <terrain_defs.h>

using namespace tiledef;
using namespace std;

//Okay dude, come on.  I know this was temporary, but come on...

class Menu {
    public:
        int selection;
        int width;
        int height;
        Tile border;
        bool done_selecting;
        string title;
        vector<string> options;
        
        bool out_of_bounds(int);

        Menu(int, Tile, vector<string>, string);
        void move_selection(int);
        void make_selection();
        bool is_done_selecting();
        //For now, this takes care of its own
        //rendering and knows about SDL. I will likely
        //regret this very soon.
        void render(SDL_Surface*, SDL_Surface*);
        void add_item(string);
        int get_selection();
        int get_max_width(vector<string>);
};

#endif
