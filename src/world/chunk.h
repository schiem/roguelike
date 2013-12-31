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
#include <vector>
#include <stdlib.h>
#include <procedurally_blind_db.h>
#include <corruptible_pblind_db.h>
#include <dungeon.h>
#include <overworld.h>
#include <terrain_defs.h>
#include <ctime>
#include <iostream>
#include <spawner.h>

using namespace tiledef;
using namespace std;
class Chunk{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        bool initialized;
        int depth;
        //Dungeon* dungeon;
        vector<Dungeon> dungeon_floors;
        Overworld overworld;
    public:
        Chunk(int, int, MapTile);
        void build_land_chunk();
        void build_water_chunk();
        void build_beach_chunk();
        IntPoint get_up_stair(int) const;
        IntPoint get_down_stair(int) const;
        int height;
        int width;
        void set_tile(int, int, int, Tile*);
        Chunk();
        const std::vector<std::vector<Tile> >& get_floor(int);
        Tile* get_tile(int, int, int) ;
        int get_depth() const;
        bool is_initialized() const;
        bool out_of_bounds(int, int, int) const;
        Spawner get_spawner(int);
        void dungeon_dump(int);
};

#endif
