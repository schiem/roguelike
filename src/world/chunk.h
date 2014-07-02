/**
 *  @file CHUNK.H
 *  @author Michael and Seth Yoder
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

#ifndef _CHUNK_H
#define _CHUNK_H

#define CHUNK_DIR DATADIR "/chunk"

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <assert.h>

#include <constants.h>
#include <procedurally_blind_db.h>
#include <corruptible_pblind_db.h>
#include <chunk_layer.h>
#include <overworld_gen.h>
#include <defs.h>
#include <ctime>
#include <iostream>
#include <spawner.h>
#include <int_point.h>

using namespace std;
namespace fs=boost::filesystem;

/**
 * The Chunk class is the largest data model in the game.
 *
 * Each Chunk contains a vector of dungeons and an overworld. Chunks are in
 * charge of their own loading method: When a new chunk is constructed or its
 * init() method is called (the first implies the second), it will look in the
 * chunk data folder to see if the chunk at its coordinates has previously been
 * serialized. If not, the chunk controls its own creation, and if so, the chunk
 * calls its deserialize() method, loading it with the appropriate data.
 */
class Chunk{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        /**
         * This chunk's height in tiles (the number of rows)
         */
        int height;

        /**
         * This chunk's width in tiles (the number of columns).
         */
        int width;

        /**
         * An integer representing chunk depth. Chunk depth is equal to the
         * number of dungeons beneath the overworld. This means that ocean and
         * sand tiles have a depth of 0. If a chunk consists of a forest tile
         * with 4 dungeons below it, the chunk has a depth of 4.
         */
        int chunk_depth;

        /**
         * The index of the chunk's row on the world map.
         */
        int world_row;

        /**
         * The index of the chunk's column on the world map.
         */
        int world_col;

        /**
         * The type of this chunk, found in defs.h.
         * @see MapTile
         */
        MapTile chunk_type;

        /**
         * A vector containing the layers of this chunk. Generally this is the
         * main data chokepoint in the Chunk class.
         */
        std::vector<ChunkLayer> layers;

        /**
         * The entry point for deserialization. Will attempt to find an
         * offloaded chunk with the given coordinates. If an offloaded chunk is
         * found in the filesystem, calls the deserialize() function and returns
         * true. Otherwise, returns false.
         *
         * @param row - The world row of the chunk to find.
         * @param col - The world column of the chunk to find.
         * @return whether or not a serialized chunk was found.
         * @see deserialize
         */
        bool find_serialized_chunk(int row, int col);

        /**
         * Loads this chunk's information from a file. Essentially does a simple
         * reverse of the serialization process. The "header" bytes, which
         * always must exist in the file, are processed one-by-one and loaded
         * into the class member variables to restore the data. Then, data which
         * may or may not exist (such as stair/spawner locations) is loaded
         * based on header information. Next, the bytes of the dungeon and
         * overworld are loaded in a triple-nested for-loop.
         *
         * @param file_name - The name of the file from which the chunk will be
         * loaded.
         * @param world_row - This chunk's row on the world map.
         * @param world_col - This chunk's column on the world map.
         */
        void deserialize(string file_name, int world_row, int world_col);

    public:
        /**
         * Empty constructor. \todo check if this can be empty.
         */
        Chunk();

        /**
         * Typical use case constructor. Will handle file deserialization by
         * itself: When provided with a location, the function will decide
         * whether or not it should deserialize an offloaded chunk.
         *
         * @param tile_type - the type of the map tile on which this chunk lies.
         * @param _world_col - The row of this chunk's location on the world
         * map.
         * @param _world_col - The column of this chunk's location on the world
         * map.
         *
         * @see MapTile
         */
        Chunk(MapTile tile_type, int _world_row, int _world_col);

        /**
         * Resets all important information in the chunk, deserializing as
         * needed.
         *
         * @param tile_type - The type of the map tile on which this chunk lies.
         * @param _world_row - The row of this chunk's location on the world
         * map.
         * @param _world_col - The column of this chunk's location on the world
         * map.
         * @see deserialize
         * @see MapTile
         */
        void init(MapTile tile_type, int _world_row, int _world_col);

        /**
         * \todo this documentation
         */
        bool build_chunk_with_dungeons();

        /**
         * Builds a chunk with dirt, trees, and dungeons.
         */
        void build_land_chunk();

        /**
         * Builds a chunk with grass, trees, groves, and dungeons.
         */
        void build_forest_chunk();

        /**
         * Builds a chunk with only a watery overworld.
         */
        void build_water_chunk();

        /**
         * Builds a chunk with only a sandy overworld.
         */
        void build_beach_chunk();

        /**
         * Returns an IntPoint representing the chunk's location on the world
         * map.
         * \todo Just use an IntPoint to store world location in general.
         * @return the chunk's location on the world map
         */
        IntPoint get_world_loc() const;

        /**
         * Returns the location of the up stair in the chunk as an IntPoint.
         *
         * @param depth - the depth of this chunk.
         */
        IntPoint get_up_stair(int depth) const;

        /**
         * Returns the location of the down stair in the chunk as an IntPoint.
         *
         * @param depth - the depth of this chunk.
         */
        IntPoint get_down_stair(int depth) const;

        /**
         * Returns a vector of all the items on the ground in this chunk.
         * @param depth - the depth of this chunk.
         * @return a pointer to a vector of items in the chunk.
         * \todo should this return a reference instead of a pointer?
         */
        std::vector<Item*>* get_items(int depth);

        /**
         * Will remove the given item from the chunk.
         * @param item - a pointer to the item to remove
         * @param depth - the depth at which to remove the item.
         */
        void remove_item(Item* item, int depth);

        /**
         * Will add the given item to the chunk's item list.
         * @param item - a pointer to the item to add
         * @param depth - the depth at which to add the item.
         */
        void add_item(Item* item, int depth);

        /**
         * @param depth - the depth to get (-1 is the overworld)
         * @return a reference to the TileMatrix of the given floor
         */
        const std::vector<std::vector<Tile> >& get_floor(int depth);

        /**
         * @param depth
         * @param row
         * @param col
         * @return a pointer to the tile at the given location.
         */
        Tile* get_tile(int depth, int row, int col) ;

        /**
         * Sets a tile at the given position to the given tile type. If the
         * depth is -1, the tile will be set at the overworld.
         *
         * @param depth - the depth of this chunk.
         * @param row - the row of the tile to set.
         * @param col - the column of the tile to set.
         * @param tile - the tile type.
         */
        void set_tile(int depth, int row, int col, Tile tile);

        /**
         * Determine how many layers are beneath the overworld in this chunk.
         * @return the depth.
         */
        int get_depth() const;

        /**
         * Given a point, will determine whether the point is out of bounds.
         * @param depth
         * @param row
         * @param col
         * @return true if the given location is out of bounds on the chunk.
         */
        bool out_of_bounds(int depth, int row, int col) const;

        /**
         * Returns the spawner object at the given depth.
         * @param depth
         * @return the spawner object
         */
        Spawner get_spawner(int depth);

        /**
         * Prints a graphical representation of the given layer to stdout.
         * @param depth - the depth of the layer to print.
         */
        void dungeon_dump(int depth);

        /**
         * @return the type of chunk.
         */
        MapTile get_type();

        /**
         * Saves all of the important information for this chunk in a file. The
         * file size is first carefully calculated, then a char array is created
         * with that size and populated with the dungeon data. The file is saved
         * in data/chunk.
         *
         * Some bitwise arithmetic is used to pack ints and bools together in a
         * single byte. As we get over 128 tile IDs, we will have to change this
         * functionality.
         */
        void serialize();

};

#endif
