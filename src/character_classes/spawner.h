/**
 *  @file SPAWNER.H
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

#ifndef SPAWNER_H
#define SPAWNER_H
#include <enemy.h>
#include <ctime>
#include <iostream>
#include <defs.h>

/** 
 * A class responsible for creating enemies.
 * This class spawns enemies at a given location.  
 * Each spawner only spawns one type of enemy.
 */

class Spawner
{
    protected:
        /**
         * X coordinate of the spawner.
         */
        int x;
        
        /**
         * Y coordinate of the spawner.
         */
        int y;
        
        /**
         * Depth of the spawner in the chunk.
         */
        int depth;
        
        /**
         * The type of enemy to spawn.
         */
        EnemyType enemy;

    public:
        /*
         * The default constructor.
         */
        Spawner();
        
        /*
         * The constructor for the spanwer.
         */
        Spawner(int, int, int,  EnemyType);
        
        /*
         * Determines whether or not an enemy should spawn.
         * Currently based entireliy on a 1/100 random chance of spawning/
         game tick.
         * \todo Make this more complex based on enemy type and other factors.
         * @return Bool depending on whether or not an enemy should spawn.
         */
        bool should_spawn();
        /**
         * Spawns a new enemy.
         * @param chunk_x The x component of the chunk where the enemy should spawn.
         * @param chunk_y The y component of the chunk where the enemey should spawn.
         * @return A new enemy based on the EnemyType.
         */
        Enemy* spawn_creep(int chunk_x, int chunk_y);
        
        /**
         * Public accessor for the member x.
         */
        int get_x();

        /**
         * Public accessor for the member y.
         */
        int get_y();
        
        /**
         * Public accessor for the depth.
         */
        int get_depth();
};

#endif

