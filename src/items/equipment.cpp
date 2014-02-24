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

Equipment::Equipment(int _weight, Tile _sprite, std::string _name, int bp, int t, int ac) : Item(_weight, _sprite, _name)
{
    body_part = bp;
    type = t;
    armor_class = ac;
}

Equipment::Equipment(EquipType eqp)
{
    weight = eqp.weight;
    sprite = eqp.sprite;
    name = eqp.name;
    body_part = eqp.body_part;
    type = eqp.type;
    armor_class = eqp.armor_class;
}


void Equipment::perform_action(int act_num, Character* character)
{
    switch(act_num)
    {
        case 0:
            equip(character);
            break;
        case 1:
            remove(character);
            break;
        default:
            break;
    }
}

void Equipment::equip(Character* character)
{
    character->set_armor(character->get_armor() + armor_class);
}

void Equipment::remove(Character* character)
{
    character->set_armor(character->get_armor() - armor_class);
}


