/**
 *  @file DUNGEONBUILDER.H
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

#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H


#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h> //floor, ceil
#include <assert.h>

#include <int_point.h>
#include <room.h>
#include <defs.h>
#include <dungeon.h>
#include <ASCII_Lib.h>

using namespace std;

/**
 * A class from which all dungeon-building classes inherit.
 *
 * This class will usually not be used directly, but only through its
 * subclasses. It provides core functionality that those dungeon builders share.
 *
 * @see ProcedurallyBlindDB
 * @see CorruptiblePBlindDB
 */
class DungeonBuilder
{

    friend ostream& operator<<(ostream&, const DungeonBuilder&);
    protected:
        /**
         * "Typical" rooms are about this large.
         */
        static const int STD_ROOM_WIDTH=8;
        static const int STD_ROOM_HEIGHT=6;

        static const int MAX_PATH_LENGTH=75;
        static const int MIN_PATH_LENGTH=16;

        /**
         * Rooms can only deviate by this width and height.
         * Keep these two numbers even.
         */
        static const int ROOM_WIDTH_DEV=2;
        static const int ROOM_HEIGHT_DEV=2;

        /**
         * The width of the dungeon to be created.
         */
        int width;

        /**
         * The height of the dungeon to be created.
         */
        int height;

        /**
         * The number of rooms in this dungeon.
         * @see Room
         */
        int num_rooms;

        /**
         * The dungeon object that this instance of DungeonBuilder will twerk
         * on.
         *
         * @see Dungeon
         */
        Dungeon main_dungeon;

        /**
         * Has an n% chance of returning 'true', where n is the given number.
         *
         * @param given - the desired probability that this check will return
         * true.
         * @return True if a random roll between 1 and 100 landed below the
         * given number.
         */
        bool rolled_over(int given) const;

        /**
         * Check if the given point on the dungeon model can be written over.
         * @param point - the point to check.
         * @return True if the given point is empty.
         */
        bool is_empty_space(IntPoint point) const;

        /**
         * Determines whether or not the given point is beyond the bounds of the
         * dungeon.
         * @param point - the point to check
         * @return True if the point is out of bounds.
         */
        bool point_is_beyond_bounds(IntPoint point) const;

        /**
         * Determines whether the edges of a given room collide with another
         * room.
         * @param r - the room to check
         * @return A binary string that denotes which edges of the room found
         * collisions; the first bit is the top edge, then right, then bottom,
         * then left.
         *
         * \todo maybe use a bitset here.
         */
        string edges_collide_with_something(Room &r) const;


        /**
         * Determines which wall of a room a given point lies on.
         * @param point - a point which has previously been determined to lie on
         * the edge of a room wall.
         *
         * @return An integer denoting the wall that the given room lies on,
         * where 0 = top, 1 = right, 2 = bottom, 3 = left.
         */
        int determine_which_wall(IntPoint point) const;

        /**
         * Builds a room in the dungeon with the given points.
         * @param tl - the top-left corner of the room.
         * @param br - the bottom-right corner of the room.
         * @return The room that was built.
         * @see room
         */
        Room build_room(IntPoint tl, IntPoint br);

        /**
         * Finds a random tile on the circumference of the given room.
         * @param current_room - the room to use.
         * @return A random IntPoint on the circumference of this room.
         */
        IntPoint rand_wall_block(const Room&);

        /**
         * Finds the adjacent point in the given direction
         * @param this_point - the current point.
         * @param direction - the direction to move, where 0 = up, 1 = right, 2 =
         * down, and 3 = left.
         * @return The point that was found.
         */
        IntPoint get_next_point(IntPoint, int) const;

        /**
         * Resets the num_rooms and main_dungeon variables, effectively clearing
         * the dungeon. The dungeon must be initialized before calling this
         * method.
         */
        virtual void reset();
    private:
        /**
         * Sets the given point to a ROOM_WALL tile if it is not a PATH tile.
         * @param row
         * @param col
         */
        void set_wall_if_not_path(int row, int col);

    public:
        Dungeon* get_dungeon();
};

#endif
