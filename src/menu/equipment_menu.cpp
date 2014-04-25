/**
 *  EQUIPMENT_MENU.CPP
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

EquipmentMenu::EquipmentMenu(int padding, Tile _border, string _title, Game* _game) : Menu(padding, _border)
{
    game = _game;
    next_screen = GAME_SCREEN;
    construct_menu();
    height = options.size() + padding;
    width = get_max_width(options) + padding;
    title = _title;
}

Menu* EquipmentMenu::make_selection()
{
    if(selection == options.size() - 1)
    {
        return new MainMenu(1, BLOCK_WALL, "Main Menu", game);
    }
    else if (options[selection] != "Not equipped")
    {
        return new EquipMenu(1, BLOCK_WALL, "Equipment", game, selection);
    }
    return this;
}

void EquipmentMenu::construct_menu()
{
    vector<Item*>* items = game->main_char.get_equipment();
    for(int i=0;i<items->size();i++)
    {
        if(items->at(i) != NULL)
        {
            options.push_back(BODY_PARTS[i] + std::string(": ")+items->at(i)->get_name());
        }
        else
        {
            options.push_back(BODY_PARTS[i] + std::string(": None"));
        }
    }
    options.push_back("Back");
} 
