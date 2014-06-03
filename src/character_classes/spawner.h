/**
 *  SPAWNER.H
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

#ifndef SPAWNER_H
#define SPAWNER_H
#include <enemy.h>
#include <ctime>
#include <iostream>
#include <enemy_defs.h>

class Spawner
{
    protected:
        int x;
        int y;
        int depth;
        EnemyType enemy;

    public:
        Spawner();
        Spawner(int, int, int,  EnemyType);
        bool should_spawn();
        Enemy* spawn_creep(int, int);
        int get_x();
        int get_y();
        int get_depth();
};

#endif

