/**
 *  @file CORRUPTIBLE_PBLIND_DB.H
 *  @author Seth A. Yoder
 *
 *  @section LICENSE
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

#ifndef _CORRUPTIBLE_PBLIND_DB_H
#define _CORRUPTIBLE_PBLIND_DB_H

#include <procedurally_blind_db.h>


/**
 * CORRUPTIBLE PROCEDURALLY-BLIND DUNGEON GENERATION
 *
 * This variant on a procedurally-blind dungeon has a more decayed look. Its
 * walls are often caved in. This is achieved by passing over the created
 * dungeon with an algorithm that modifies cells according to their surroundings
 * and random chance.
 */
class CorruptiblePBlindDB : public ProcedurallyBlindDB
{
    private:
        /**
         * Performs the following transformation to all given room corners,
         * where percentages represent the chance that a certain transformation
         * will occur. This function is called by corrupt_walls().
         *
         * \verbatim
         * O = empty
         * X = wall
         * . = room interior
         * ? = anything
         *
         *   O O O   15%  O O O
         *   O X X  --->  O O X
         *   O X .        O X X
         *     |
         *     |     15%  O O O
         *     \------->  O O X
         *                O X .
         * \endverbatim
         *
         * @param corners - A vector of IntPoints representing room corners.
         * @see corrupt_walls
         */
        void corrupt_corners(vector<IntPoint> corners);

        /**
         * After a dungeon is built, will "corrupt" the dungeon walls according
         * to the following rules:
         * \verbatim
         * O = empty
         * X = wall
         * . = room interior
         * ? = anything
         *
         *   O O O   15%  O O O
         *   O X X  --->  O O X
         *   O X .        O X X
         *     |
         *     |     15%  O O O
         *     \------->  O O X
         *                O X .
         *
         *   O O O   5%   O O O
         *   X X X  --->  X O X
         *   ? . ?        X X X
         *     |
         *     |     5%
         *     |------->  O O O
         *     |          X O X
         *     |          . X .
         *     |
         *     |     5%
         *     |------->  O O O
         *     |          X O X
         *     |          . X X
         *     |
         *     |     5%
         *     \------->  O O O
         *                X O X
         *                X X .
         * \endverbatim
         */
        void corrupt_walls();

    public:
        /**
         * Primary constructor. Will not actually design the dungeon.
         *
         * @param _width - the desired width of the dungeon.
         * @param _height - the desired height of the dungeon.
         *
         * @see ProcedurallyBlindDB
         */
        CorruptiblePBlindDB(int _width, int _height);

        /**
         * The primary entry point for dungeon building. Overrides the parent
         * class's method of the same room, and adds a call to corrupt_walls().
         *
         * @param target - The target number of rooms to build.
         * @param deviation - The standard room count deviation.
         * @see corrupt_walls()
         */
        void build_dungeon(int target, int deviation);
};

#endif
