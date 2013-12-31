/**
 *  DUNGEON.H
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
//#define ROOM_COUNT_DEBUG
#include <vector>
#include <assert.h>
#include <terrain_defs.h>
#include <room.h>
#include <spawner.h>

class Dungeon
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        TileMatrix dungeon;
        void tile_assertions(int, int) const;
        Spawner spawner;

    public:
        int width;
        int height;
        int num_rooms;
        IntPoint down_stair;
        IntPoint up_stair;
        std::vector<Room> rooms;
        void make_stairs(bool);
        void make_border();
        Tile get_tile(int, int) const;
        Tile get_tile(IntPoint) const;
        Tile* get_tile_pointer(int, int);
        void set_tile(int, int, Tile);
        void set_tile(IntPoint, Tile);
        const std::vector<std::vector<Tile> >& get_dungeon();
        Dungeon(int, int);
        Dungeon(const Dungeon&);
        Dungeon();
        Dungeon& operator= (const Dungeon& d);
        Spawner get_spawner();
        void dungeon_dump();
        void make_spawner(int);
};

#endif
