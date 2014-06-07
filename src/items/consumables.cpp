/**
 *  CONSUMABLES.CPP
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

Consumable::Consumable(int _rarity, int _weight, Tile _sprite, std::string _name, IntPoint _coords, int stat, int val, int _type) : Item(_rarity, _weight, _sprite, _name, _coords)
{
    stat_modified = stat;
    amount_modified = val;
    type = _type;
    can_equip = false;   
    can_use = false;
    can_wield = false;
    can_consume = true;
}

Consumable::Consumable(IntPoint _coords, ConsumableType cons) : Item(_coords)
{
    rarity = cons.rarity;
    weight = cons.weight;
    sprite = cons.sprite;
    description = cons.description;
    name = cons.name;
    stat_modified = cons.stat;
    amount_modified = cons.value;
    can_use = cons.use;
    type = cons.type;
    can_equip = false;
    can_wield = false;
    can_consume = true;
}


void Consumable::perform_action()
{
    if(can_use)
    {
    }
}

int Consumable::get_stat()
{
    return stat_modified;
}

int Consumable::get_amount()
{
    return amount_modified;
}

int Consumable::get_type()
{
    return type;
}
