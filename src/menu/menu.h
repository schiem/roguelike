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
#include <game_states.h>

using namespace tiledef;
using namespace std;

/*
Each different type of menu is going to be a subclass that overloads the make_selection() function 
to provide different functionality.  See the documentation for make_selection for more detail.
*/

class Menu {
    public:
        Screen next_screen;
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
        
        /*
        This is the most important function.  There are three possibilities:
        1.  The selection just transitions to a new menu.  The menu returned will
            be the new menu.
        2.  The selection just performs a function and then returns to the same menu.
            In this case, "this" will be returned and nothing else will happen.
        3.  The selection causes an exit of the menu screen.  When this happens, 
            "this" will be returned and should_exit will be toggled. On the next pass,
            the game_state will then transition to the next_screen.

        There's no reason that 2 & 3 can't be combined.
        */
        virtual Menu* make_selection() = 0;
        //For now, this takes care of its own
        //rendering and knows about SDL. I will likely
        //regret this very soon.
        
        //Don't worry, I got it bro -M
        void add_item(string);
        bool should_exit();
        int get_selection();
        int get_max_width(vector<string>);
        void toggle_exit();
        Screen get_screen();
};

class StartMenu : public Menu
{
    public:

        StartMenu(int, Tile, string);
        Menu* make_selection();
};

class MainMenu: public Menu
{
    public:
        MainMenu(int, Tile, string);
        Menu* make_selection();
};

class EquipmentMenu: public Menu
{
    public: 
        EquipmentMenu(int, Tile, string);
        Menu* make_selection();
};


#endif
