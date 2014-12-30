/**
 *  @file WEAPON.CPP
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

Weapon::Weapon()
{}

Weapon::Weapon(int _rarity, int _weight, Tile _sprite, std::string _name, IntPoint _coords, int t,  int _dam) : Item(_rarity, _weight, _sprite, _name, _coords)
{
    type = t;
    damage = _dam;
    can_equip = false;   
    can_use = false;
    can_wield = true;
    can_consume = false;
}

Weapon::Weapon(IntPoint _coords, WeaponType wpn) : Item(_coords)
{
    rarity = wpn.rarity;
    weight = wpn.weight;
    sprite = wpn.sprite;
    name = wpn.name;
    description = wpn.description;
    damage = wpn.damage;
    range = wpn.range;
    type = wpn.type;
    size = wpn.size;
    can_use = wpn.use;
    can_equip = false;
    can_wield = true;
    can_consume = false;
}


void Weapon::perform_action()
{
    if(can_use)
    {
    }
}

int Weapon::get_type()
{
    return type;
}


int Weapon::get_damage()
{
    return damage;
}
