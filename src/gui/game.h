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
#include <chunk_matrix.h>
#include <constants.h>
#include <overworld.h>
#include <bresenham.h>
#include <enemy.h>
#include <dungeonbuilder.h>
#include <dungeon.h>
#include <defs.h>
#include <character.h>
#include <world_map.h>
#include <helper.h>
#include <item.h>

struct TilePoint {
    Tile tile;
    IntPoint loc;
};

class Game
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;

    private:

        //BASE Data/Models
        /**
         * Whether or not the game has been initialized.
         */
        bool initialized;
        
        /**
         * If the game is currently paused.
         * This isn't a game state because things still need to
         * be happening in other game states while the game is paused.
         */
        bool paused;

        /**
         * The map of the world.
         * @see MapTile
         */
        MapTileMatrix world_map;
        
        /**
         * A two dimentsional vector containing the chunks.
         * This is the core of how the game doesn't run out of 
         * memory.  It's broken up into chunks, which are then
         * serialized/deserialized as the player moves about.
         * @see Chunk
         */
        ChunkMatrix chunk_map;
        
        /**
         * Is this still here?
         * \todo Do something else with this...
         */
        Tile block_wall_tile;

        //BASE model access functions
        /**
         * Assertions to ensure that the current point is in the screen.
         * @param row The row of the point.
         * @param col The column of the point.
         */
        void point_assertions(int row, int col);
        
        /**
         * Sets the point in the canvas at row, col to Tile
         * @param row The row of the canvas to set the tile.
         * @param col The column of the canvas to set the tile.
         * @param tile The tile to set row, col to.
         */
        void set_tile(int row, int col, Tile* tile);
        
        /**
         * Sets the point in the canvas at IntPoint to tile.
         * @param point The point of the canvas to set the tile.
         * @param tile The tile to set the IntPoint of the canvas to.
         */
        void set_tile(IntPoint point, Tile* tile);
        
        /**
         * Gets the item at the specific coordinates.
         * @param coords The coordinates in the chunk.
         * @param chunk The chunk of the item.
         * @return The item at the coordinates. NULL if none is found.
         */
        Item* item_at_coords(IntPoint, IntPoint, int);

        /**
         * Public accessor for the tile at canvas[row][col].
         */
        Tile* get_tile(int row, int col);

        /**
         * Public accessor for the tile at canvas[point.row][point.col].
         */
        Tile* get_tile(IntPoint point);
        
        /**
         * Checks if the point at (row, col) is out of the screen.
         */
        bool out_of_bounds(IntPoint point);
        bool out_of_bounds(int, int);
        bool in_buffer(int, int);
        bool in_range(IntPoint, IntPoint, IntPoint, IntPoint, IntPoint);
        void update_main_char_chunk();
        IntPoint get_canvas_coords(IntPoint, IntPoint);
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
        std::vector<Enemy*> enemy_list;

        //ENEMY Functionality

public:
        Game();
        ~Game();
        void init(const MapTileMatrix&, IntPoint);

        //BASE Data/Models
        Main_Character main_char;

        //BASE Model access
        Chunk* get_current_chunk();
        bool is_initialized();
        void act(long);
        bool is_paused();
        void pause();
        void unpause();
        void toggle_pause();
        //RENDERING Functionality
        const std::vector<std::vector<Tile*> >& get_canvas();
        IntPoint get_vis_coords(IntPoint, IntPoint);
        void show_vis_items();
        std::vector<Enemy*> get_vis_enemies();
        void refresh();

        //ENEMY Functionality
        void run_spawners();
        void run_enemies(long);
        void spawn_enemy(int, int, int, int, int, int);
        std::vector<Enemy*>* get_enemies();
        //MAIN CHAR Functionality
        void change_main_depth(int);
        void move_main_char(int, int);
        void get_item(); 
        void drop_item(Item*);
};

#endif
