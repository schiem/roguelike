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

class Menu {
    public:
        int selection;
        string title;
        int width;
        int height;
        Tile border;
        vector<string> options;
        bool exit; 
        bool out_of_bounds(int);

        Menu(int, Tile);
        void move_selection(int);
        virtual void make_selection() = 0;
        //For now, this takes care of its own
        //rendering and knows about SDL. I will likely
        //regret this very soon.
        
        //Don't worry, I got it bro -M
        void add_item(string);
        bool should_exit();
        int get_selection();
        int get_max_width(vector<string>);
        void toggle_exit();
};

class StartMenu : public Menu
{
    public:

        StartMenu(int, Tile, string);
        void make_selection();
};
#endif
