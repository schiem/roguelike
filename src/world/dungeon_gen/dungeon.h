#ifndef _DUNGEON_H
#define _DUNGEON_H
#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#define MAX_ROOMS 15
#include <vector>
#include <assert.h>
#include <terrain_defs.h>
#include "room.h"

class Dungeon
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
       TileMatrix dungeon;

    public:
       int width;
       int height;
       int num_rooms;
       std::vector<int> down_stair;
       std::vector<int> up_stair;
       std::vector<Room> rooms;
       void make_stairs(bool);
       void tile_assertions(int, int) const;
       Tile get_tile(int, int) const;
       Tile get_tile(IntPoint) const;
       void set_tile(int, int, Tile);
       void set_tile(IntPoint, Tile);
       const std::vector<std::vector<Tile> >& get_dungeon();
       Dungeon(int, int);
       Dungeon(const Dungeon&);
       Dungeon();
       Dungeon& operator= (const Dungeon& d);
};

#endif
