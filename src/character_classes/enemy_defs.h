/**
 *  ENEMY_DEFS.H
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

#ifndef ENEMY_DEFS_H
#define ENEMY_DEFS_H

#include <terrain_defs.h>
#include <item_defs.h>
#include <vector>
#include <string>
#include <stdarg.h>
#include <ctime>
#include <stdlib.h>
#include <boost/assign/list_of.hpp>

struct EnemyType
{
    int moral;
    int max_health;
    int base_attack;
    int armor;
    int id;
    int sight;
    int speed;
    std::string name;
    Tile sprite;
    Tile corpse;
    std::vector<EquipType> equip_list;
    std::vector<WeaponType> wep_list;
    bool operator==(const EnemyType& rhs) const
    {
        return this->id==rhs.id;
    }
};

namespace enemies
{
    extern EnemyType kobold;
    extern EnemyType rabbit;
}

#endif
