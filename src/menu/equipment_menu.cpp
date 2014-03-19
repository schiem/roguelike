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

EquipmentMenu::EquipmentMenu(int padding, Tile _border, string _title) : Menu(padding, _border)
{
    next_screen = GAME_SCREEN;
    title = _title;
    options.push_back("head");
    options.push_back("torso");
    options.push_back("arms");
    options.push_back("back");
    height = options.size() + padding;
    width = get_max_width(options) + padding;
}

Menu* EquipmentMenu::make_selection()
{
    if(selection == options.size() - 1)
    {
        return new MainMenu(1, BLOCK_WALL, "Main Menu");
    }
    return this;
}
