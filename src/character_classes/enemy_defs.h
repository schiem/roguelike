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
#include <string>

struct EnemyType
{
    int id;
    std::string name;
    int max_health;
    Tile sprite;
    Tile corpse;
    int sight;
    int speed;
    int moral;
    int attack;
    bool operator==(const EnemyType& rhs) const 
    {
        return this->id==rhs.id;
    }
};

namespace enemies
{
    extern EnemyType Kobold;
}

#endif
