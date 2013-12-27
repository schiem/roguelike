#ifndef _CANVAS_H
#define _CANVAS_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <chunk.h>
#include <overworld.h>
#include <bresenham.h>
#include <enemy.h>
#include <enemy_defs.h>
#include "dungeonbuilder.h"
#include "dungeon.h"
#include "terrain_defs.h"
#include "character.h"


using namespace enemies;
using namespace std;
using namespace tiledef;
class Game
{
    typedef std::vector<std::vector<Chunk> > ChunkMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    private:
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        TilePointerMatrix canvas;
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
        void undo_visibility();
        bool in_buffer(int, int);
        bool in_visible(IntPoint, IntPoint);
        TilePointerMatrix get_surroundings(IntPoint, IntPoint, int);
        void top_layer_append(IntPoint, IntPoint, Tile);
        IntPoint get_abs(IntPoint, IntPoint);
    
    public:
        Game();
        DungeonBuilder db;
        Main_Character main_char;
        void run_spawners();
        void run_enemies();
        void refresh();
        void resize_canvas(int, int);
        const std::vector<std::vector<Tile*> >& get_matrix();
        Chunk* get_chunk();
        const std::vector<std::vector<Tile> > get_top_layer();
        void change_main_depth(int);
        void move_main_char(int, int);
};

#endif
