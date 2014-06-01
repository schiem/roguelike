/**
 *  ITEM_DEFS.H
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

#ifndef ITEM_DEFS_H
#define ITEM_DEFS_H

#include <terrain_defs.h>
#include <string>

struct EquipType
{
    int weight;
    Tile sprite;
    std::string name;
    int rarity;
    int body_part;
    int type;
    int armor_class;
    bool use;
    bool operator==(const EquipType& rhs) const 
    {
        return this->name==rhs.name;
    }
};

struct WeaponType
{
    int weight;
    Tile sprite;
    std::string name;
    int rarity;
    int damage;
    int type;
    int range;
    bool use;
    bool operator==(const EquipType& rhs) const 
    {
        return this->name==rhs.name;
    }
};


namespace equipment
{
    extern EquipType boots;
}

namespace weapons
{
    extern WeaponType dagger;
}

#endif
