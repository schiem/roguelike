#ifndef _CANVAS_H
#define _CANVAS_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>
#include <assert.h>
#include <chunk.h>
#include <overworld.h>
#include <bresenham.h>
#include <enemy_defs.h>
#include <enemy.h>
#include <dungeonbuilder.h>
#include <dungeon.h>
#include <terrain_defs.h>
#include <character.h>

struct TilePoint {
    Tile tile;
    IntPoint loc;
};

class Game
{
    typedef std::vector<std::vector<Chunk> > ChunkMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    private:
        //BASE Data/Models
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        ChunkMatrix chunk_map;
        std::vector<TilePoint> top_layer;
        IntPoint main_char_chunk;
        Tile block_wall_tile;

        //BASE model access functions
        void point_assertions(int, int);
        void set_tile(int, int, Tile*);
        void set_tile(IntPoint, Tile*);
        Tile* get_tile(int, int);
        Tile* get_tile(IntPoint);
        bool out_of_bounds(IntPoint);
        bool out_of_bounds(int, int);
        bool in_buffer(int, int);
        bool in_visible(IntPoint, IntPoint);
        void top_layer_append(IntPoint, IntPoint, Tile);
        void update_main_char_chunk();
        IntPoint get_abs(IntPoint, IntPoint);
        IntPoint get_buffer_coords(IntPoint, IntPoint);
        TileMatrix get_surroundings(IntPoint, IntPoint, int);

        //RENDERING Data/Models
        TilePointerMatrix canvas;
        TilePointerMatrix buffer;
        std::vector<std::vector<IntPoint> > bresenham_lines;

        //RENDERING Functionality
        void recalculate_visibility_lines(int radius);
        void update_buffer(IntPoint);
        void update_chunk_map(IntPoint);
        void draw_visibility_lines();
        void undo_visibility();

        //ENEMY Data/Models
        std::vector<Enemy> enemy_list;

        //ENEMY Functionality

public:
        Game();

        //BASE Data/Models
        Main_Character main_char;

        //BASE Model access
        Chunk* get_current_chunk();
        const std::vector<TilePoint> get_top_layer();

        //RENDERING Functionality
        const std::vector<std::vector<Tile*> >& get_canvas();
        void refresh();

        //ENEMY Functionality
        void run_spawners();
        void run_enemies();

        //MAIN CHAR Functionality
        void change_main_depth(int);
        void move_main_char(int, int);

};

#endif
