/**
 *  SPAWNER.CPP
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

#include <spawner.h>
using namespace std;
Spawner::Spawner()
{
    }

Spawner::Spawner(int _x, int _y, int _depth,  EnemyType _enemy)
{
    x = _x;
    y = _y;
    depth = _depth;
    enemy = _enemy;
}

bool Spawner::should_spawn()
{
    //TODO: put in a switch{case} for this
    return (rand() % 100 == 0);
}

Enemy Spawner::spawn_creep(int chunk_x, int chunk_y)
{
    return Enemy(enemy, x - 1, y - 1, chunk_x, chunk_y, depth);
}

int Spawner::get_x()
{
    return x;
}

int Spawner::get_y()
{
    return y;
}

int Spawner::get_depth()
{
    return depth;
}
