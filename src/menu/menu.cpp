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

#include "menu.h"


Menu::Menu(int padding, Tile _border) {
    border = _border;
    selection = 0;
    exit=false;
}

bool Menu::out_of_bounds(int row) {
    return ((row < 0) || (row >= options.size()));
}

/**
 * Changes the selection menu option.
 */
void Menu::move_selection(int direction) {
    int new_selection = selection + direction;
    //That rhymed.
    if(!out_of_bounds(new_selection)) {
        selection = new_selection;
    }
}


int Menu::get_selection() {
    return selection;
}

void Menu::add_item(string new_item)
{
    options.push_back(new_item);
}

int Menu::get_max_width(vector<string> string_list)
{
    int length = 0;
    for(int i=0; i<string_list.size(); i++)
    {
        if(string_list[i].size() > length)
        {
            length = string_list[i].size();
        }
    }
    return length;
}

bool Menu::should_exit()
{
    return exit;
}

void Menu::toggle_exit()
{
    exit = !exit;
}

Screen Menu::get_screen()
{
    return next_screen;
}

