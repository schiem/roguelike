/**
 *  @file DUNGEON.H
 *  @author (Seth|Michael) Yoder
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

#ifndef _DUNGEON_H
#define _DUNGEON_H
#define MAX_ROOMS 15
#include <vector>
#include <stdlib.h>
#include <assert.h>

#include <defs.h>
#include <room.h>
#include <spawner.h>
#include <item.h>
#include <enemy.h>

/**
 * Represents an underground dungeon using a two-dimensional vector as a base
 * model. This also keeps track of rooms that are in the dungeon.
 *
 * @see Room
 */
class Dungeon
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        /**
         * The base model, a simple two-dimensional vector.
         */
        TileMatrix dungeon;

        /**
         * The dungeon's monster spawner. Every dungeon has them... for now.
         */
        Spawner spawner;

        /**
         * An array of items that are contained within this dungeon.
         */
        std::vector<Item*> items;

        /**
         * @param row
         * @param col
         * Run the given point in the dungeon through a series of assertions
         * to ensure that it is valid.
         */
        void tile_assertions(int row, int col) const;

        /**
         * Copies over all values from the given dungeon to this dungeon.
         * @param d - The dungeon from which to swap ownership.
         */
        void swap(const Dungeon& d);


    public:
        /**
         * Dungeon width in tiles
         */
        int width;

        /**
         * Dungeon height in tiles
         */
        int height;

        int num_rooms;

        /**
         * The coordinates of the stair leading to a deeper floor, if there is
         * one.
         */
        IntPoint down_stair;

        /**
         * The coordinates of the stair leading to the layer above this one, if
         * such a layer exists.
         */
        IntPoint up_stair;

        /**
         * The location of this dungeon's spawner
         */
        IntPoint spawner_loc;

        /**
         * A vector containing data about the rooms in this dungeon.
         */
        std::vector<Room> rooms;

        /**
         * Empty constructor. Initializes with width 10 and height 10. Probably
         * should never use explicitly.
         */
        Dungeon();

        /**
         * @param _width the width of the dungeon
         * @param _height the height of the dungeon
         *
         * Creates a new dungeon with the given width and height.
         */
        Dungeon(int _width, int _height);

        /**
         * @param d a reference to another dungeon
         * (Copy constructor)
         */
        Dungeon(const Dungeon& d);

        /**
         * @param d a reference to another dungeon
         * @return a reference to this dungeon
         *
         */
        Dungeon& operator= (const Dungeon& d);

        /**
         * @param has_layer_below a bool denoting whether or not there is a
         * layer below this dungeon.
         *
         * Create an "up stair" in a random room and a "down stair" in a
         * random room (if there is a chunk below).
         */
        void make_stairs(bool has_layer_below);

        /**
         * @param row
         * @param col
         * @return a tile at the given location.
         */
        Tile get_tile(int row, int col) const;

        /**
         * @param point
         * @return a tile at the given location.
         */
        Tile get_tile(IntPoint point) const;

        /**
         *  @param row
         *  @param col
         *  @return a pointer to the tile at the given location.
         */
        Tile* get_tile_pointer(int row, int col);

        /**
         * @param point
         * @return a pointer to the tile at the given location.
         */
        Tile* get_tile_pointer(IntPoint point);

        /**
         * @param row
         * @param col
         * @param theTile
         */
        void set_tile(int row, int col, Tile theTile);

        /**
         * @param point
         * @param theTile
         */
        void set_tile(IntPoint point, Tile theTile);

        /**
         * @return a const reference to the dungeon floor model.
         */
        const std::vector<std::vector<Tile> >& get_dungeon();

        /**
         * @param depth the depth at which to build a spawner
         *
         * Builds a monster spawner in a random room at the given depth.
         */
        void make_spawner(int depth);

        /**
         * @return the monster spawner in this dungeon.
         */
        Spawner get_spawner();

        /**
         * @return a pointer to the vector of items in this dungeon.
         */
        std::vector<Item*>* get_items();

        /**
         * @param item the item to add
         * Adds the given item to the list of items in this dungeon.
         */
        void add_item(Item* item);

        /**
         * Prints an ASCII representation of the dungeon to stdout.
         */
        void dungeon_dump();

};

#endif
