/**
 *  @file MAIN_MENU.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
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

StartMenu::StartMenu(int padding, Tile _border) : Menu(padding, _border)
{
    id = menu_id::START_MENU;
    next_screen = MAP_SCREEN;
    title = "POOPBUTTS: THE RECKONING";
    options.push_back("New Game");
    options.push_back("Continue");
}

Menu* StartMenu::make_selection() {
    switch(selection) {
        case 0:
            toggle_exit();
            return this;
            break;
        case 1:
            return new LoadMenu(1, BLOCK_WALL);
            break;
        default:
            toggle_exit();
            return this;
            break;
    }
}
