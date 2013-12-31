/**
 *  CHUNK.H
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
#include <map>
#include <string.h>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>

#include <procedurally_blind_db.h>
#include <corruptible_pblind_db.h>
#include <dungeon.h>
#include <overworld.h>
#include <terrain_defs.h>
#include <ctime>
#include <iostream>
#include <spawner.h>
#include <int_point.h>

using namespace tiledef;
using namespace std;
namespace fs=boost::filesystem;

class Chunk{
    typedef std::multimap<std::time_t, fs::path> result_set_t;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        bool initialized;
        int depth;
        //Dungeon* dungeon;
        vector<Dungeon> dungeon_floors;
        Overworld overworld;

        string find_serialized_chunk(int, int);
        IntPoint parse_file_name(string);


    public:
        int height;
        int width;

        Chunk();
        Chunk(int, int, MapTile, int, int);
        void build_land_chunk();
        void build_water_chunk();
        void build_beach_chunk();
        IntPoint get_up_stair(int) const;
        IntPoint get_down_stair(int) const;
        void set_tile(int, int, int, Tile*);
        const std::vector<std::vector<Tile> >& get_floor(int);
        Tile* get_tile(int, int, int) ;
        int get_depth() const;
        bool is_initialized() const;
        bool out_of_bounds(int, int, int) const;
        Spawner get_spawner(int);
        void dungeon_dump(int);
        void serialize(int, int);
        void deserialize(int, int);
};

#endif
