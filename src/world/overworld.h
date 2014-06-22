/**
 *  @file OVERWORLD.H
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

#ifndef _OVERWORLD_H
#define _OVERWORLD_H

#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <spawner.h>
#include <int_point.h>
#include <bresenham.h>
#include <item.h>
#include <defs.h>

/**
 * Represents the top layer of a chunk. If and when we graduate to fully
 * 3-dimensional worlds, this will probably be the first class to go.
 */
class Overworld
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        /**
         * The central data model for the class: A two-dimensional matrix
         * storing the overworld tiles.
         */
        TileMatrix ground;

        /**
         * The monster spawner in this overworld chunk. If there is none, it
         * will just remain uninitialized.
         */
        Spawner spawner;

        /**
         * Almost a copy of the method of the same name from the WorldMap class.
         * @see WorldMap::smoothing_pass
         *
         * @param threshold - The number of surrounding tiles of tile_type must
         * be greater than this value in order for any change to occur.
         * @tile-type - The type of tile that we have a chance of changing to
         * @num - The number of tiles of tile_type surrounding the tile in
         * focus.
         *
         * @return true if this tile should be changed to tile_type.
         */
        bool smoothing_pass(int threshold, Tile tile_type, int num);

        /**
         * A vector storing pointers to each item on the chunk.
         */
        std::vector<Item*> items;

        /**
         * A vector storing all equipment on the chunk.
         */
        std::vector<EquipType> equipment;

        /**
         * All weapons on the chunk.
         */
        std::vector<WeaponType> weapons;
    public:
        Overworld(int _width, int _height, bool has_layer_below, MapTile tile_type);
        Overworld();

        /**
         * True if there is a layer below this one.
         */
        bool has_layer_below;

        /**
         * An IntPoint representing the location of the down stair on this
         * chunk, if such a stair exists. If it does not exist, the IntPoint
         * will stay uninitialized.
         */
        IntPoint down_stair;

        /**
         * The width of this chunk.
         */
        int width;

        /**
         * The height of this chunk.
         */
        int height;

        /**
         * Returns the tile at the given (row, col) location.
         *
         * @param row
         * @param col
         * @return a pointer to the tile at that location
         */
        Tile* get_tile(int row, int col) ;

        /**
         * Sets the tile at the given location.
         *
         * @param row
         * @param col
         * @param tile_type
         */
        void set_tile(int row, int col, Tile tile_type);

        /**
         * Constructs an overworld of "land" type on this chunk. "Land" has dirt
         * and some trees.
         */
        void build_land_overworld();

        /**
         * Constructs an ocean on this chunk. Every tile becomees a water tile.
         */
        void build_water_overworld();

        /**
         * Constructs a beach on this chunk. Every tile becomes a type of sand
         * tile.
         */
        void build_beach_overworld();

        /**
         * Constructs a forest on this chunk. Forests have dark grass floors,
         * thick trees, and tree-lined oases.
         */
        void build_forest_overworld();

        /**
         * @return a reference to the matrix of tiles on this chunk.
         */
        std::vector<std::vector<Tile> >& get_ground();

        /**
         * @return a pointer to the list of item pointers on this chunk.
         */
        std::vector<Item*>* get_items();

        /**
         * Adds an item to the item list for this chunk.
         * @param item - a pointer to the item to add.
         */
        void add_item(Item* item);

        /**
         * @return a vector of the equipment on this chunk.
         */
        std::vector<EquipType> get_equipment();

        /**
         * @return a vector of the weapons on this chunk.
         */
        std::vector<WeaponType> get_weapons();

        /**
         * @return the spawner on this chunk. Will return junk values if there
         * is no spawner on the chunk!
         */
        Spawner get_spawner();


};

#endif
