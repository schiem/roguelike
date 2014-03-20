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

StartMenu::StartMenu(int padding, Tile _border, string _title) : Menu(padding, _border)
{
    next_screen = MAP_SCREEN;
    construct_menu();
    normalize_options();
    height = options.size() + padding;
    width = get_max_width(options) + padding;
    title = _title;
}

Menu* StartMenu::make_selection()
{
   toggle_exit();
   return this;
}

void StartMenu::construct_menu()
{
    options.push_back("New Game");
    options.push_back("Continue");
}
