#ifndef _DUNGEON_H
#define _DUNGEON_H
#define MAX_ROOMS 15
//#define ROOM_COUNT_DEBUG
#include <vector>
#include <assert.h>
#include <terrain_defs.h>
#include "room.h"
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
        std::vector<int> down_stair;
        std::vector<int> up_stair;
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
