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
        Chunk(int, int);
        std::vector<int> get_up_stair(int) const;
        std::vector<int> get_down_stair(int) const;
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
