#ifndef _CANVAS_H
#define _CANVAS_H

#define STARTING_WIDTH 100
#define STARTING_HEIGHT 50

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>
#include <chunk.h>
#include <overworld.h>
#include "dungeonbuilder.h"
#include "dungeon.h"
#include "terrain_defs.h"
#include "character.h"

using namespace std;
using namespace tiledef;
class Canvas 
{
    typedef std::vector<std::vector<Chunk> > ChunkMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        int chunk_x;
        int chunk_y;
        std::vector<std::vector<Tile> > canvas;
        std::vector<std::vector<Chunk> > chunk_map; //temporary, we'll need to serialize this
        std::vector<std::vector<Tile> > buffer;
        void update_buffer();
        void update_chunk();
        bool out_of_bounds();
    public:
        Canvas();
        //TODO do something about these two. Events.cpp?
        //The way the project is now structured, yeah.  Typical games consist of 5 sections:
        //init, input, process, display, cleanup.  As it is, we've started dividing those into files, but we don't have it the whole way yet. Most of those are sitting in Display, and could be given their own files. 
        DungeonBuilder db; 
        Main_Character main_char;
        void refresh();
        void resize_canvas(int, int);
        const std::vector<std::vector<Tile> >& get_matrix();
        const Chunk& get_chunk();
    
};

#endif
