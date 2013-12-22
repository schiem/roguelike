#ifndef _OVERWORLD_H
#define _OVERWORLD_H
#define MAX_WIDTH 500 
#define MAX_HEIGHT 500

#include <terrain_defs.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iostream>

class Overworld
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        TileMatrix ground;

    public:
        bool is_dungeon;
        std::vector<int> down_stair;
        int width;
        int height;
        Tile* get_tile(int, int) ;
        void set_tile(int, int, Tile*);
        Overworld(int, int, bool);
        Overworld();
        std::vector<std::vector<Tile> >& get_ground();
};

#endif
