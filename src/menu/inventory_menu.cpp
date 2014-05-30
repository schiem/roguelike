/**
 *  INVENTORY_MENU.CPP
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

InventoryMenu::InventoryMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = 6;
    game = _game;
    next_screen = GAME_SCREEN;
    items = game->main_char.get_inventory();
    construct_menu();
    height = options.size() + padding;
    width = get_max_width(options) + padding;
    title = "Inventory";
}

Menu* InventoryMenu::make_selection()
{
    if(selection == options.size() - 1)
    {
        return new MainMenu(1, BLOCK_WALL, game);
    }
    else
    {
        return new ItemMenu(1, BLOCK_WALL, game, items->at(selection)); 
    }
    return this;
}

void InventoryMenu::construct_menu()
{
    for(int i=0; i<items->size();i++)
    {
        options.push_back(items->at(i)->get_name());
    }
    options.push_back("Back");
}

