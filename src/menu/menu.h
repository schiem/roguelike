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
#include <defs.h>
#include <game_states.h>
#include <item.h>
#include <game.h>
#include <character.h>
#include <helper.h>

using namespace tiledef;
using namespace std;

/*
Each different type of menu is going to be a subclass that overloads the make_selection() function 
to provide different functionality.  See the documentation for make_selection for more detail.
*/

class Menu {
    public:
        Game* game;
        Screen next_screen;
        int selection;
        string title;
        Tile border;
        vector<string> options;
        bool exit; 
        bool out_of_bounds(int);
        int id;
        Menu(int, Tile);
        void move_selection(int);
        int padding; 
        vector<string> extra_lines;
        
        /*
        This is the most important function.  There are four possibilities:
        1.  The selection just transitions to a new menu.  The menu returned will
            be the new menu.
        2.  The selection just performs a function and then returns to the same menu.
            In this case, "this" will be returned and nothing else will happen.
        3.  The selection causes an exit of the menu screen.  When this happens, 
            "this" will be returned and should_exit will be toggled. On the next pass,
            the game_state will then transition to the next_screen.
        4.  The function returns NULL, in which case the game is exited.
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
        void toggle_exit();
        Screen get_screen();
        int get_id();
        int num_extra_lines();
        vector<string> get_extra_lines();
};

class StartMenu : public Menu
{
    public:

        StartMenu(int, Tile);
        Menu* make_selection();
        
};

class MainMenu: public Menu
{
    public:
        MainMenu(int, Tile, Game*);
        Menu* make_selection();
        

};

class EquipmentMenu: public Menu
{
    public: 
        EquipmentMenu(int, Tile, Game*);
        Menu* make_selection();
        

};

class InventoryMenu: public Menu
{
    protected:
        vector<Item*>* items;
    public:
        InventoryMenu(int, Tile, Game*);
        Menu* make_selection();
        
};

class ItemMenu: public Menu
{
    protected:
        Item* item;
    public:
        ItemMenu(int, Tile, Game*, Item*);
        Menu* make_selection();
        
};

class EquipMenu: public Menu
{
    protected:
        int item;
    public:
        EquipMenu(int, Tile, Game*, int);
        Menu* make_selection();
        
};

class EscapeMenu: public Menu
{
    public:
        EscapeMenu(int, Tile, Game*);
        Menu* make_selection();
        
};

class AudioMenu: public Menu
{
    public:
        AudioMenu(int, Tile, Game*);
        Menu* make_selection();
        
};

class FontMenu: public Menu
{
    private:
        string font;
    public:
        FontMenu(int, Tile, Game*);
        Menu* make_selection();
        string get_font();
};

class InfoMenu: public Menu
{
    protected:
        Item* item;
    public:
        InfoMenu(int, Tile, Game*, Item*);
        Menu* make_selection();
};

#endif
