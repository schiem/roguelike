/**
 *  EQUIPMENT.CPP
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

#include <item.h>
#include <character.h>
using namespace tiledef;
using namespace equipment;

Equipment::Equipment(int _weight, Tile _sprite, std::string _name, IntPoint _coords, int bp, int t, int ac) : Item(_weight, _sprite, _name, _coords)
{
    body_part = bp;
    type = t;
    armor_class = ac;
    can_equip = true;   
    can_use = false;
}

Equipment::Equipment(IntPoint _coords, EquipType eqp) : Item(_coords)
{
    weight = eqp.weight;
    sprite = eqp.sprite;
    name = eqp.name;
    body_part = eqp.body_part;
    type = eqp.type;
    armor_class = eqp.armor_class;
    can_use = eqp.use;
    can_equip = true;
    cout<<can_use<<endl;
}


void Equipment::perform_action()
{
    if(can_use)
    {
    }
}

int Equipment::get_body_part()
{
    return body_part;
}
