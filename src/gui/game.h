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

#include <vector>
#include <chunk_matrix.h>
#include <constants.h>
#include <enemy.h>
#include <world_map.h>
#include <helper.h>
#include <item.h>
#include <animation.h>
#include <animation_defs.h>

//Forward declarations
struct Tile;
struct Item;

class Chunk;
class Character;
class Spawner;
class Den;
class Plant;

namespace td=tiledef;

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
         * The name of this saved game. Maybe eventually it will be based on a
         * randomly-generated world name, but for now it is generated based on
         * the date and time.
         */
        string name;

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
         * A two dimensional vector containing the chunks.
         * This is the core of how the game doesn't run out of
         * memory.  It's broken up into chunks, which are then
         * serialized/deserialized as the player moves about.
         * @see Chunk
         */
        ChunkMatrix chunk_map;

        /**
         * Holds any tiles that we need to make pointers to.
         */
        Tile tile_index[td::TILE_TYPE_COUNT];
        
        /**
         * Whenever a new animation needs to be performed, it
         * should be added to this queue.
         */
        std::vector<Animation> anim_queue;

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
         * Checks if the point at (point.row, point.col) is out of the screen.
         */
        bool out_of_bounds(IntPoint point);

        /**
         * Checks if the point at (row, col) is out of the screen.
         */
        bool out_of_bounds(int row, int col);

        /**
         * Returns whether or not the chunk is currently in the buffer.
         */
        bool in_buffer(int row, int col);
        
        /**
         * Checks whether a set of coordinates (relative to the buffer)
         * is inside the buffer.
         */
        bool coords_in_buffer(int row, int col);

    
        /**
         * Checks if one point is within a certain radius of another.
         * @param chunk The chunk of the point to check.
         * @param coords The coords of the point to check.
         * @param range_chunk The chunk of the second point to check.
         * @param center The coords of the second chunk to check.
         * @param radius The radius around the first check.
         * @return Whether or not the point is in range.
         */
        bool in_range(IntPoint chunk, IntPoint coords, IntPoint range_chunk, IntPoint center, IntPoint radius);

        /**
         * What does this do? I have no idea.
         */
        void update_main_char_chunk();
        IntPoint get_canvas_coords(IntPoint, IntPoint);
        IntPoint get_buffer_coords(IntPoint, IntPoint);
        Character* enemy_at_loc(IntPoint, IntPoint);

        //RENDERING Data/Models
        TilePointerMatrix canvas;
        TilePointerMatrix buffer;
        std::vector<std::vector<IntPoint> > bresenham_lines;


        //ENEMY Data/Models
        //TODO get rid of the character_list
        std::vector<Character*> character_list;
        std::vector<Character*> character_queue;

        //ENEMY Functionality

    public:
        Game();
        void init(const MapTileMatrix&, IntPoint);

        //BASE Data/Models
        Character main_char;

        //BASE Model access
        Chunk* get_current_chunk();
        string get_name();
        void set_name(string n);
        bool is_initialized();
        void act(long);
        bool is_paused();
        void pause();
        void unpause();
        void toggle_pause();
        bool visibility_on;
        //RENDERING Functionality
        const std::vector<std::vector<Tile*> >& get_canvas();
        IntPoint get_vis_coords(IntPoint, IntPoint);
        std::vector<Character*> get_vis_characters();
        
        
        /**
         * The function that writes objects from the chunk (items, 
         * plants, etc.) into the buffer to be rendered.
         */
        void show_chunk_objects(); 
       
        /**
         * Adds a tile to the buffer at the given coordinates, after
         * checking if the tile is in the buffer.
         */
        void add_tile_to_buffer(IntPoint chunk, IntPoint coords, Tile* tile);
        
        /**
         * Checks if a point in the form of chunk, coords is in the buffer.
         */
        bool point_in_buffer(IntPoint chunk, IntPoint coords);

        /**
         * Writes a list of items into the buffer at a given chunk 
         * location.
         */
        void items_to_buffer(std::vector<Item*>* items, IntPoint chunk);

        /**
         * Writes a list of spawners into the buffer at a given chunk 
         * location.
         */
        void spawners_to_buffer(std::vector<Spawner>* spawners, IntPoint chunk);
        
        /**
         * Writes the tiles in a den into the buffer.
         */
        void den_to_buffer(Den* den, IntPoint chunk, IntPoint coords);
        
        /**
         * Writes plants to the buffer.
         */
        void plants_to_buffer(std::vector<Plant>* plants, IntPoint chunk);
        
        std::vector<Animation>& get_animations();
        void tick_animations(long delta_ms); 
        bool is_vis(IntPoint coords);
        void refresh();
        void recalculate_visibility_lines(int radius);
        void update_buffer(IntPoint);
        void update_chunk_map(IntPoint);
        void draw_visibility_lines();
        void undo_visibility();

        //ENEMY Functionality
        void run_spawners();
        void clear_character_queue();
        std::vector<Character*> flush_characters(); 
        
        void remove_targets(Character* enem);
        std::vector<Character*>& get_characters();
        
        /**
         * Returns true if there is an enemy in range of the character.
         * If so, sets the character's target to the enemy found.
         */
        bool enemy_in_range(Character* chara);
        
        /**
         * Uses pathfinding to get to the point.  Note: this could be
         * a nightmare, as characters will currently only attempt to move
         * to points they can see, but the pathfinding may take them on
         * a very bizarre path involving the entire buffer.
         */
        int move_to_point(Character* chara, IntPoint coords, IntPoint chunk);
    
        /**
         * Causes the enemy to try to run away.
         */
        int run_away(Character* chara, IntPoint coords, IntPoint chunk);

        /**
         * Causes a character to wander listlessly.
         */
        void wander(Character* chara);
        
        /**
         * Checks if one character is next to another.
         */
        bool next_to_char(Character* chara, Character* target);

        /**
         * Kills a character.  DOES NOT DELETE THE POINTER.
         */
        void kill(Character* chara);
        
        /**
         * Removes an enemy from the game. DOES NOT DELETE THE POINTER.
         */
        void remove_enemy(Character* chara);

        /**
         * Checks if a character is in the buffer.
         */
        bool character_in_range(Character* chara);
        
        //MAIN CHAR Functionality
        void change_depth(int, Character*);
        bool move_char(int, int, Character*);
        
        /**
         * Causes one character to attack another.
         */
        bool attack_char(Character* chara, Character* target); 
        void get_item(Character*);
        void drop_item(Item*, Character*);

        //DEBUG FUNCTIONS
        void spawn_enemy(int, int, int, int, int, int);

        /**
         * Debug function to teleport the character to a new place.
         */
        void teleport(int chunk_x, int chunk_y, int x, int y);

        //ANIMATION FUNCTIONS
        void create_explosion(int x, int y, int chunk_x, int chunk_y);
};

#endif
