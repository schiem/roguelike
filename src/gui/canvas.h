#ifndef _CANVAS_H
#define _CANVAS_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <chunk.h>
#include <overworld.h>
#include <bresenham.h>
#include "dungeonbuilder.h"
#include "dungeon.h"
#include "terrain_defs.h"
#include "character.h"

using namespace std;
using namespace tiledef;
class Canvas
{
    typedef std::vector<std::vector<Chunk> > ChunkMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        TilePointerMatrix canvas;
        //temporary, we'll need to serialize this
        //unless we decide to make the world finite and cap it.
        ChunkMatrix chunk_map;
        TilePointerMatrix buffer;
        TileMatrix top_layer;
        IntPoint main_char_chunk;
        std::vector<std::vector<IntPoint> > bresenham_lines;
        void recalculate_visibility_lines(int radius);
        void point_assertions(int, int);
        void set_tile(int, int, Tile*);
        void set_tile(IntPoint, Tile*);
        Tile* get_tile(int, int);
        Tile* get_tile(IntPoint);
        bool out_of_bounds(IntPoint);
        bool out_of_bounds(int, int);
        void update_buffer(IntPoint);
        void update_chunk_map(IntPoint);
        void update_chunk();
        void draw_visibility_lines();
    public:
        Canvas();
        //TODO do something about these two. Events.cpp?  The way the project is
        //now structured, yeah.  Typical games consist of 5 sections: init,
        //input, process, display, cleanup.  As it is, we've started dividing
        //those into files, but we don't have it the whole way yet. Most of
        //those are sitting in Display, and could be given their own files.
        DungeonBuilder db;
        Main_Character main_char;
        void refresh();
        void resize_canvas(int, int);
        const std::vector<std::vector<Tile*> >& get_matrix();
        Chunk* get_chunk();
        const std::vector<std::vector<Tile> > get_top_layer();
        void change_main_depth(int);
        void move_main_char(int, int);
};

#endif
