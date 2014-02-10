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
#include <helper.h>


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
        int STARTING_WIDTH;
        int STARTING_HEIGHT;
        bool initialized;
        MapTileMatrix world_map;
        ChunkMatrix chunk_map;
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
        bool in_range(IntPoint, IntPoint, IntPoint, IntPoint, IntPoint);
        void update_main_char_chunk();
        IntPoint get_buffer_coords(IntPoint, IntPoint);
        TileMatrix get_surroundings(IntPoint, IntPoint, int, IntPoint);
        std::vector<Character*> nearby_enemies(IntPoint, IntPoint, IntPoint); 
        Character* enemy_at_loc(IntPoint, IntPoint);

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
        Game(int, int);
        void init(const MapTileMatrix&, IntPoint);

        //BASE Data/Models
        Main_Character main_char;

        //BASE Model access
        Chunk* get_current_chunk();
        bool is_initialized();
        void act(long);

        //RENDERING Functionality
        const std::vector<std::vector<Tile*> >& get_canvas();
        IntPoint get_vis_coords(IntPoint, IntPoint);
        std::vector<Enemy> get_vis_enemies();
        void refresh();

        //ENEMY Functionality
        void run_spawners();
        void run_enemies(long);

        //MAIN CHAR Functionality
        void change_main_depth(int);
        void move_main_char(int, int);

};

#endif
