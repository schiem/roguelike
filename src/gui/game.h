/**
 *  GAME.H
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CANVAS_H
#define _CANVAS_H

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
#include <world_map.h>

struct TilePoint {
    Tile tile;
    IntPoint loc;
};

class Game
{
    typedef std::vector<std::vector<Chunk> > ChunkMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;

    private:
        //BASE Data/Models
        bool initialized;
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        MapTileMatrix world_map;
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
        void init(const MapTileMatrix&, IntPoint);

        //BASE Data/Models
        Main_Character main_char;

        //BASE Model access
        Chunk* get_current_chunk();
        const std::vector<TilePoint> get_top_layer();
        bool is_initialized();

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
