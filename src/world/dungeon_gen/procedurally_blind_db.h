/**
 *  @file PROCEDURALLY_BLIND_DB.H
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

#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include <dungeonbuilder.h>

/**
 * Generates dungeons in a "procedurally-blind" manner.
 *
 * The method for this dungeon generation technique is:
 *
 * 1. Create a viable "first" dungeon room.
 * 2. Tunnel out from that room in a number of paths.
 * 3. At the end of the last path that was created, try to create another room.
 * 4. Repeat this until one of two conditions is met:
 *      a. The room limit is reached
 *      b. There is no viable room space at the end of the path.
 * 5. If the resulting dungeon does not have very many rooms, scrap it and start
 * over. This currently happens <25% of the time.
 */
class ProcedurallyBlindDB : public DungeonBuilder
{
    protected:
        /**
         * Builds an initial room in the dungeon space.
         * @see find_viable_starting_point
         */
        void build_start_room();

        /**
         * Will find and return a viable room space, and if there is no such space,
         * will return a room with coordinates (-1,-1),(-1,-1).
         *
         *  Pseudocode for this method:
         *
         *  Declare min room width and min room height; declare and define test_room
         *  based on this width and height; declare upper_bound, lower_bound,
         *  left_bound, right_bound = 1
         *
         *  if test_room collides with something solid: return something nullish
         *
         *  while (room width < max) and (room height < max) AND (upper_bound +
         *  lower_bound + left_bound + right_bound > 0):
         *
         *      move all of test_room's points out; (subroutine probably) subtract
         *      upper_bound from row value of both upper points; subtract left_bound
         *      from col value of both left side points; add lower_bound to row
         *      value of both lower points; add right_bound to col value of both
         *      right side points;
         *
         *      "scan" across those newly created edges;
         *
         *      if, during the scan, we ran into a solid block: which side did it
         *      occur on?  Set the *_bound to 0 for that side; move the points on
         *      that side one step toward the room center;
         *
         * @param the_point - the point from which the function will start
         * attempting to find a room space.
         *
         * @return a room representing the discovered room space, or
         * ((-1,-1),(-1,-1)) if there is no such room.
         */
        Room find_viable_room_space(IntPoint the_point) const;

        /**
         * Will find a starting point for the dungeon.
         * @param std_width - The "standard" width for the dungeon.
         * @param std_height - The "standard" height for the dungeon.
         * \todo since std_width is a constant, do we need a parameter here?
         *
         * @return the point that was found.
         */
        IntPoint find_viable_starting_point(int std_width, int std_height) const;

        /**
         * Builds a path starting at the given point.
         *
         * Will attempt to build a path starting from the given point, and
         * return the point where the path ends; if the path cannot be built,
         * returns IntPoint(-1, -1).
         *
         * @param start - the starting point for this path.
         * @param direction - The starting direction of the path, where 0 = up,
         * 1 = right, 2 = down, and 3 = left.
         * @return the end of the path, or (-1,-1) if the path could not be
         * built.
         */
        IntPoint build_path(IntPoint start, int direction);

        /**
         * A recursive helper function to build the dungeon.
         *
         * Finds a viable wall block in the given room, building paths
         * outward from those wall blocks and often building rooms at the ends
         * of those paths. Every time a new room is built, the function is
         * called again with that room's index passed as 
         *
         * @param target - The target number of rooms.
         * @param deviation - The standard room number deviation.
         * \todo Deviation should be a const int in DungeonBuilder.
         */
        void build_dungeon_recursive(int target, int deviation);

    public:
        /**
         * Empty constructor. Should never really be called.
         * \todo Remove things in ProcedurallyBlindDB constructor.
         */
        ProcedurallyBlindDB();
        
        /**
         * Primary constructor. Will not build the dungeon, but will initialize
         * one.
         *
         * @param _width - The width of the dungeon to build.
         * @param _height - The height of the dungeon to build.
         */
        ProcedurallyBlindDB(int _width, int _height);

        /**
         * The primary entry point for dungeon building.
         *
         * @param target - The target number of rooms to build.
         * @param deviation - The standard room count deviation.
         */
        virtual void build_dungeon(int target, int deviation);
};

#endif
