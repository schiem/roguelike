#ifndef _DUNGEON_H
#define _DUNGEON_H
#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#define MAX_ROOMS 15
#include <terrain_defs.h>
#include "Room.h"
#include <vector>

class Dungeon
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        //TODO
       //Tile dungeon[MAX_WIDTH][MAX_HEIGHT];
       TileMatrix dungeon;

    public:
       int width;
       int height;
       int num_rooms;
       std::vector<Room> rooms;
       Tile get_tile(int, int) const;
       Tile get_tile(IntPoint) const;
       void set_tile(int, int, Tile);
       void set_tile(IntPoint, Tile);
       const std::vector<std::vector<Tile> >& get_dungeon();
	   Dungeon(int, int);
       Dungeon(const Dungeon&);
       Dungeon();
	
};

#endif
