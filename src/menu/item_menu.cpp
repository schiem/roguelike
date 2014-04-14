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

ItemMenu::ItemMenu(int padding, Tile _border, string _title, Game* _game, Item* _item) : Menu(padding, _border)
{
    game = _game;
    item = _item;
    next_screen = GAME_SCREEN;
    construct_menu();
    height = options.size() + padding;
    width = get_max_width(options) + padding;
    title = _title;
}

Menu* ItemMenu::make_selection()
{
    cout<<options[selection]<<endl;
    if(options[selection] == "Use")
    {
        //use the item...not entirely sure what this will look like.
    }
    else if(options[selection] == "Equip")
    { 
        cout<<"Equipping"<<endl;
        game->main_char.equip_item(item);
    }
    else if(options[selection] == "Drop")
    {
        game->drop_item(item);
    }
    else if(options[selection] == "Destroy")
    {
        game->main_char.destroy_item(item);
    }
    return new InventoryMenu(1, BLOCK_WALL, "Inventory", game);
}

void ItemMenu::construct_menu()
{
    if(item->can_use)
    {
        options.push_back("Use");
    }
    if(item->can_equip)
    {
        options.push_back("Equip");
    }
    options.push_back("Drop");
    options.push_back("Destroy");
    options.push_back("Back");
}

