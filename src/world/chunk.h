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

using namespace tiledef;
using namespace std;
class Chunk{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        bool initialized;
        int x;
        int y;
        int depth;
        //Dungeon* dungeon;
        vector<Dungeon> dungeon_floors;
        Overworld overworld;
    public:
        Chunk(int, int, int, int);
        int height;
        int width;
        Chunk();
        const std::vector<std::vector<Tile> >& get_floor(int);
        Tile get_tile(int, int, int) const;
        int get_depth() const;
        int get_x() const;
        int get_y() const;
        bool is_initialized() const;

};

#endif
