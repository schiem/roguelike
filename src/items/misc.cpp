/**
 *  @file MISC.CPP
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

#include <item.h>
#include <character.h>
using namespace tiledef;
using namespace equipment;

Misc::Misc()
{}

Misc::Misc(int _rarity, int _weight, Tile _sprite, std::string _name, IntPoint _coords) : Item(_rarity, _weight, _sprite, _name, _coords)
{
    can_equip = false;   
    can_use = false;
    can_wield = false;
    can_consume = false;
}

Misc::Misc(IntPoint _coords, MiscType misc) : Item(_coords)
{
    rarity = misc.rarity;
    weight = misc.weight;
    sprite = misc.sprite;
    description = misc.description;
    name = misc.name;
    can_use = misc.use;
    size = misc.size;
    can_equip = false;
    can_wield = false;
    can_consume = false;
}


void Misc::perform_action()
{
    if(can_use)
    {
    }
}
