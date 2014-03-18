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
using namespace std;


Menu::Menu(int padding, Tile _border, std::vector<string> _options, string _title) {
    height = _options.size() + padding;
    width = get_max_width(_options) + padding;
    border = _border;
    selection = 0;
    done_selecting = false;
    options = _options;
    title = _title;
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

void Menu::make_selection() {
    done_selecting = true;
}

bool Menu::is_done_selecting() {
    return done_selecting;
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
