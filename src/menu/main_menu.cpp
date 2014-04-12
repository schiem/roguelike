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

MainMenu::MainMenu(int padding, Tile _border, string _title, Character* new_char) : Menu(padding, _border)
{
    next_screen = GAME_SCREEN;
    character = new_char;
    construct_menu();
    normalize_options();
    height = options.size() + padding;
    width = get_max_width(options) + padding;
    title = _title;
    cout<<character<<endl;
}

Menu* MainMenu::make_selection()
{
    switch (selection)
    {
        case 0:
            return new EquipmentMenu(1, BLOCK_WALL, "Equipment Menu", character);
            break;
        case 1:
            return new InventoryMenu(1, BLOCK_WALL, "Inventory Menu", character);
            break;
        case 2:
            return this;
            break;
        case 3:
            toggle_exit();
            return this;
            break;
        default:
            toggle_exit();
            return this;
            break;
    }
}

void MainMenu::construct_menu()
{
    options.push_back("Equipment");
    options.push_back("Inventory");
    options.push_back("Stats");
    options.push_back("Return");
} 
