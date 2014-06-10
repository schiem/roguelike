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
#include <dungeon.h>
#include <overworld.h>
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
         * A vector containing the dungeons underneath the overworld on this
         * chunk. Generally this is the main data chokepoint in the Chunk class.
         */
        vector<Dungeon> dungeon_floors;

        /**
         * The overworld for this chunk.
         * @see Overworld
         */
        Overworld overworld;

        /**
         * The entry point for deserialization. Will attempt to find an
         * offloaded chunk with the given coordinates. If an offloaded chunk is
         * found in the filesystem, calls the deserialize() function and returns
         * true. Otherwise, returns false.
         *
         * @param row - The world row of the chunk to find.
         * @param col - The world column of the chunk to find.
         * @return whether or not 
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
         * 
         */
        IntPoint get_world_loc() const;
        IntPoint get_up_stair(int) const;
        IntPoint get_down_stair(int) const;
        void set_tile(int, int, int, Tile);
        std::vector<Item*>* get_items(int);
        void remove_item(Item*, int);
        void add_item(Item*, int);
        const std::vector<std::vector<Tile> >& get_floor(int);
        Tile* get_tile(int, int, int) ;
        int get_depth() const;
        bool out_of_bounds(int, int, int) const;
        Spawner get_spawner(int);
        void dungeon_dump(int);
        void serialize();
        MapTile get_type();
};

#endif
