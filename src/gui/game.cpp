/**
 *  GAME.CPP
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
#include <iostream>
#include <assert.h>
#include <stdlib.h>

#include <game.h>
#include <defs.h>
#include <chunk.h>
#include <character.h>
#include <bresenham.h>
#include <pathfinding.h>

using namespace std;
namespace td=tiledef;

/*
Explanation of how the screen works: There is a chunk map, which holds all of
the chunks.  We need a way to go from chunk->screen.  However, since we always
want the character to be centered, there will be times when the screen consists
of half of one chunk and half of another.

Let's say that this is an array of chunks:

111222333
111222333
111222333

and the screen is 3x3, to give us a view of:

222
2X2
222

with X being the character.  If the character moves left, then we get:

122
1X2
122

So, there is a buffer, which essentially holds all of the surrounding chunks.
A buffer might look like:

123
456
789

Where each number is a different chunk.  But, we only want part of this to get
written to the screen.  So, we have a canvas, which is the size of the screen
taken from the buffer with the character at the center.

*/

/* ========= PUBLIC METHODS ==========*/

/*--------------------Base Model Functions--------------------------*/
Game::Game() {
    initialized = false;
    paused = false;

    for(int i = 0; i < td::TILE_TYPE_COUNT; i++) {
        tile_index[i] = td::TILE_INDEX[i];
    }
}

Game::~Game()
{
    for(int i=0;i<character_list.size();i++)
    {
        delete character_list[i];
    }
}


void Game::init(const MapTileMatrix& _world_map, IntPoint selected_chunk) {
    world_map = _world_map;

    //Give me a buffer size of 150x300 (tiles, which are 8x16 pixels)
    //The buffer is what the screen draws from.
    buffer = TilePointerMatrix(CHUNK_HEIGHT * 3, vector<Tile*>(CHUNK_WIDTH * 3));
    character_index = std::vector<std::vector<Character*> >(CHUNK_HEIGHT * 3, std::vector<Character*>(CHUNK_WIDTH * 3));
    clear_character_index();

    //Each chunk holds an overworld and several
    //dungeons, which are generated upon chunk creation.
    //This is the "starting" chunk (arbitrary).
    
    //TODO get this clutter somewhere else
    int main_stat_array[NUM_STATS] = {100, 2, 100, 10, 10, 10}; 
    std::vector<int> main_stats(&main_stat_array[0], &main_stat_array[0] + NUM_STATS);
    main_char = Character(main_stats, 50, 25, td::MAIN_CHAR, misc::player_corpse, selected_chunk.col, selected_chunk.row, 0, 0, 70, -1);
    main_char.add_item(new Consumable(main_char.get_chunk(), consumables::potato));
    IntPoint buffer_coords = get_buffer_coords(main_char.get_chunk(), main_char.get_coords());
    character_index[buffer_coords.row][buffer_coords.col] = &main_char;


    //What gets drawn to the screen
    canvas = TilePointerMatrix(GAME_HEIGHT, vector<Tile*>(GAME_WIDTH));

    //Eventually, this should be based on screen size.
    chunk_map = ChunkMatrix(3, selected_chunk, world_map, name);
    update_buffer(main_char.get_chunk());
    recalculate_visibility_lines(15);
    refresh();

    visibility_on = true;
    initialized = true;
}

Chunk* Game::get_current_chunk() {
    return chunk_map.get_center_chunk();
    //return &chunk_map[main_char.get_chunk().row][main_char.get_chunk().col];
}

string Game::get_name() {
    return name;
}

void Game::set_name(string n) {
    name = n;
}

bool Game::is_initialized() {
    return initialized;
}

void Game::act(long delta_ms) {
    tick_animations(delta_ms);
    main_char.act(delta_ms);
}

/*---------------------Rendering Functions--------------------------*/
const std::vector<std::vector<Tile*> >& Game::get_canvas() {
    return canvas;
}

/* PRE:  Takes in an IntPoint representing chunk coordinates, an InPoint
 * representing coordinates within that chunk.  Assumes that
 * the coordinates given represent a space within the screen.
 * POST: Returns an IntPoint containing the coordinates on the screen.
 */
IntPoint Game::get_vis_coords(IntPoint chunk, IntPoint coords) {
    IntPoint temp;
    IntPoint abs = get_abs(chunk, coords);
    //tl stands for top-left
    //it should be CHUNK_HEIGHT
    IntPoint tl_abs = get_abs(main_char.get_chunk(),
            IntPoint(main_char.get_y() - CHUNK_HEIGHT/2, main_char.get_x() - CHUNK_WIDTH/2));
    temp = IntPoint(abs.row - tl_abs.row, abs.col - tl_abs.col);
    return temp;
}


std::vector<Character*> Game::get_vis_characters() {
    std::vector<Character*> temp = std::vector<Character*>();
    for(int i=0;i<character_list.size();i++) {
        IntPoint chunk = IntPoint(character_list[i]->get_chunk_y(), character_list[i]->get_chunk_x());
        IntPoint coords = IntPoint(character_list[i]->get_y(), character_list[i]->get_x());
        IntPoint main_char_coords = IntPoint(main_char.get_y(), main_char.get_x());
        IntPoint radius  = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
        if(in_range(chunk, coords, main_char.get_chunk(), main_char_coords, radius) &&
                character_list[i]->get_depth() == main_char.get_depth()) {
            temp.push_back(character_list[i]);
        }
    }
    return temp;
}


bool Game::is_vis(IntPoint coords)
{
    bool y = coords.row < GAME_HEIGHT && coords.row >= 0;
    bool x = coords.col < GAME_WIDTH && coords.col >=0;
    return x && y;
}

std::vector<Animation>& Game::get_animations()
{
    return anim_queue;
}

void Game::tick_animations(long delta_ms)
{
    for(int i=0;i<anim_queue.size();i++)
    {
        anim_queue[i].step(delta_ms);
        if(anim_queue[i].get_done())
        {
            anim_queue.erase(anim_queue.begin() + i);
        }
    }
}

/*
 * Updates the canvas with the area around the character in terms of buffer coordinates.
 * Draws visibility lines.
 * This is to refresh the screen whenever the character moves.
 */
void Game::refresh() {
    for(int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            //aight, this needs some comments becuase ain't nobody can
            //read this
            
            //if you suddenly start getting weird visual errors when you
            //try to make the buffer hold more than 3 chunks at a time,
            //this is why.  Right now, it assumes that you're always in
            //the middle chunk of 3 (+ 1 chunk_height)

            //Explanation for the following:
            //Here is the buffer:
            //123
            //456
            //789
            //You are always in chunk 5, because it updates when you move.
            //Therefore, you are always + 1 chunk_size (height or width)
            //relative to the top  of the buffer.
            //To get the screens coordinates, you add 1 chunk_size to
            //your coords, then subtract half of the screen that you want
            //to display, and then add the index.
            int buffer_tile_row = (CHUNK_HEIGHT + main_char.get_y()) -
                (GAME_HEIGHT/2) + i;
            int buffer_tile_col = (CHUNK_WIDTH + main_char.get_x()) -
                (GAME_WIDTH/2) + j;
            set_tile(i, j, buffer[buffer_tile_row][buffer_tile_col]);
        }
    }
    draw_visibility_lines();
}

/*---------------------Character Helper Functions---------------------*/
/* PRE: None
 * POST: Iterates through the chunks which are part of the buffer and runs any
 * spawner (TODO: make it so it will run any spawners, plural).  If the spawner
 * returns that it should spawn, then it will spawn an enemy based on the type
 * of spawner that it is and append it to the enemy list.
 */
void Game::run_spawners() {
    std::vector<Spawner>* spawners;
    Chunk* chunk;
    IntPoint chunk_coords;
    int accum = 0;
    for(int i=main_char.get_chunk().row-1;i<=main_char.get_chunk().row+1;i++) {
        for(int j=main_char.get_chunk().col-1;j<=main_char.get_chunk().col+1;j++) {
            chunk = chunk_map.get_chunk_abs(IntPoint(i, j));

            if(chunk->get_depth()>main_char.get_depth() && chunk->get_type().does_spawn) {
                spawners = chunk->get_spawners(main_char.get_depth());
                for(int k=0;k<spawners->size();k++)
                {
                    Spawner* spawner = &spawners->at(k);
                    spawner->run();
                    std::vector<Character*> charas = spawner->flush();
                    for(int index=0;index<charas.size();index++)
                    {
                        charas[index]->set_chunk(IntPoint(i, j));
                        character_list.push_back(charas[index]);
                        character_queue.push_back(charas[index]);
                        character_to_index(charas[index]);
                    }
                    spawner->clear_queue();
                }
            }
        }
    }
}


void Game::clear_character_queue()
{
    character_queue.clear();
}


std::vector<Character*> Game::flush_characters()
{
    return character_queue;
}

void Game::remove_targets(Character* enem)
{
    for(int i=0;i<character_list.size();i++)
    {
        if(character_list[i]->get_target() == enem)
        {
            character_list[i]->set_target(NULL);
        }
    }
    if(main_char.get_target() == enem)
    {
        main_char.set_target(NULL);
    }
}

std::vector<Character*>& Game::get_characters() {
    return character_list;
}


bool Game::enemy_in_range(Character* chara){ 
    //establish the necessary variables
    Character* best = NULL;
    Character* new_character = NULL;
    int target_id = 5 - chara->get_moral();
    int selectability = 2;
    
    std::vector<IntPoint> sight_tiles = chara->sight_tiles();
    for(int i=0;i<sight_tiles.size();i++)
    {
        IntPoint index_coords = get_buffer_coords(chara->get_chunk(), sight_tiles[i]);
        if(coords_in_buffer(index_coords.row, index_coords.col))
        {
            new_character = character_index[index_coords.row][index_coords.col];
        }
        else{
            new_character = NULL;
        }
        //Check if we care about the enemy
        
        if(new_character != NULL && new_character->get_moral() > target_id - selectability && new_character->get_moral() < target_id + selectability)
        {
            //If we don't have a target, this is the best target
            if(best == NULL)
            {
                best = new_character;
            }
            else
            {
                //otherwise, check if this one we found is better
                if((unsigned int)(new_character->get_moral() - target_id) < (unsigned int)(best->get_moral() - target_id))
                {
                    best = new_character;
                }
            }
        }
    }

    if(best != NULL)
    {
        chara->set_target(best);
        return true;
    }
    else
    {
        return false;
    }
}


int Game::move_to_point(Character* chara, IntPoint coords, IntPoint chunk)
{
    IntPoint goal = get_buffer_coords(chunk, coords);
    IntPoint current = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    IntPoint movement = pathfinding::get_next_step(goal, buffer, current, chara->get_sight());
    if((movement + current) == goal)
    {
        return 1; //success
    }
    else if(move_char(movement.col, movement.row, chara))
    {
        return 2; //running
    }
    else
    {
        return 0; //we failed :(
    }
}


int Game::run_away(Character* chara, IntPoint coords, IntPoint chunk)
{
    IntPoint current_coords = get_abs(chara->get_chunk(), chara->get_coords());
    IntPoint other_coords = get_abs(chunk, coords);
    IntPoint movement = pathfinding::get_opposite(current_coords, other_coords);
    return move_char(movement.col, movement.row, chara);
}

void Game::wander(Character* chara)
{
    
    int will_move = rand() % 5;
    int x_change = rand() % 3 - 1;
    int y_change = rand() % 3 - 1;
    IntPoint new_coords = IntPoint(chara->get_y() + y_change, chara->get_x() + x_change); 
  
    if(point_in_buffer(chara->get_chunk(), new_coords) && will_move == 0)
    {
        move_char(x_change, y_change, chara);
    }
}

bool Game::next_to_char(Character* chara, Character* target)
{
    IntPoint chara_abs = get_abs(chara->get_chunk(), chara->get_coords());
    IntPoint target_abs = get_abs(target->get_chunk(), target->get_coords());
    IntPoint difference = chara_abs - target_abs;
    bool x = difference.col <= 1 && difference.col >= -1;
    bool y = difference.row <= 1 && difference.row >= -1;
    bool same_coords = difference.col == 0 && difference.row == 0;
    return x && y && !same_coords;
}

void Game::kill(Character* chara)
{
    
    IntPoint chunk = chara->get_chunk();
    IntPoint coords = chara->get_coords();
    Item* corpse = chara->get_corpse();
    corpse->set_coords(coords);
    Chunk* current_chunk = chunk_map.get_chunk_abs(chunk.row, chunk.col);
    current_chunk->add_item(corpse, chara->get_depth());

    chara->remove_all();
    vector<Item*>* item_list = chara->get_inventory();
    for(int j=0;j<item_list->size();j++)
    {
        Item* item = item_list->at(j);
        drop_item(item, chara);
    }
    drop_item(corpse, chara);
    remove_enemy(chara);
}

void Game::remove_enemy(Character* chara)
{
    remove_targets(chara);
    for(int i=0;i<character_list.size();i++)
    {
        if(character_list[i] == chara)
        {
            character_list.erase(character_list.begin() + i);
        }
    }
    remove_index_char(chara);
    delete chara;
}


bool Game::character_in_range(Character* chara)
{
    return in_buffer(chara->get_chunk_x(), chara->get_chunk_y());
}


/*--------------------Character Controller Functions----------------------*/


void Game::change_depth(int direction, Character* chara) {
    assert(direction == -1 || direction == 1);
    Chunk * current_chunk;
    current_chunk = get_current_chunk();
    Tile* current_tile = current_chunk->get_tile(chara->get_depth(),
            chara->get_y(), chara->get_x());

    if(direction == -1) {
        if (chara->get_depth() >= 1) {
            if(*current_tile == td::UP_STAIR) {
                chara->set_depth(chara->get_depth() - 1);
                chara->set_x(current_chunk->get_down_stairs(chara->get_depth())[0].col);
                chara->set_y(current_chunk->get_down_stairs(chara->get_depth())[0].row);
            }
        }
    } else {
        if (chara->get_depth()+1 < current_chunk->get_depth()) {
            if(*current_tile == td::DOWN_STAIR) {
                chara->set_depth(chara->get_depth() + 1);
                chara->set_x(current_chunk->get_up_stairs(chara->get_depth())[0].col);
                chara->set_y(current_chunk->get_up_stairs(chara->get_depth())[0].row);
            }
        } else {
            //cout<<"DEPTH OF THIS CHUNK: "<<current_chunk->get_depth()<<endl;
        }
    }
}

bool Game::move_char(int col_change, int row_change, Character* chara) {
    if(!chara->is_conscious()) {
        return false;
    }

    int row = chara->get_y();
    int col = chara->get_x();
    int next_col = chara->get_x() + col_change;
    int next_row = chara->get_y() + row_change;

    IntPoint new_chunk = IntPoint(chara->get_chunk_y() + (next_row >= CHUNK_HEIGHT) - (next_row<0),
            chara->get_chunk_x() + (next_col>=CHUNK_WIDTH) - (next_col<0));

    next_col = next_col +  (CHUNK_WIDTH * (next_col<0)) - (CHUNK_WIDTH * (next_col>=CHUNK_WIDTH));
    next_row = next_row +  (CHUNK_HEIGHT * (next_row<0)) - (CHUNK_HEIGHT * (next_row>=CHUNK_HEIGHT));
    IntPoint next_coords = IntPoint(next_row, next_col);
    Character* enem = enemy_at_loc(new_chunk, next_coords);

    IntPoint buffer_coords = get_buffer_coords(new_chunk, IntPoint(next_row, next_col));
    bool can_move = buffer[buffer_coords.row][buffer_coords.col]->can_be_moved_through;

    if(can_move && (enem == NULL)) {
        remove_index_char(chara);
        col = next_col;
        row = next_row;
        chara->set_x(col);
        chara->set_y(row);
        if(chara->get_chunk() != new_chunk) {
            chara->set_chunk(new_chunk);
        }
        chara->reduce_endurance(1);
        character_to_index(chara);
        return true;
    } else {
        return false;
    }
}

bool Game::attack_char(Character* chara, Character* target)
{
    chara->attack(target);
    chara->set_target(target);
    chara->reduce_endurance(2);
    return true;
}


/// \todo Make this take in a character so that other characters can call it?
void Game::get_item(Character* chara)
{
    IntPoint coords = chara->get_coords();
    IntPoint chunk = chara->get_chunk();
    Item* temp_item = item_at_coords(IntPoint(chara->get_y(), chara->get_x()), chara->get_chunk(), chara->get_depth());
    Chunk* current_chunk = chunk_map.get_chunk_abs(chara->get_chunk_y(), chara->get_chunk_x());
    if(temp_item != NULL)
    {
        ///\todo Add a check if the inventory is full
        chara->add_item(temp_item);
        current_chunk->remove_item(temp_item, chara->get_depth());
        IntPoint b_coords = get_buffer_coords(chunk, coords);
        bool was_seen = buffer[b_coords.row][b_coords.col]->visible;
        buffer[b_coords.row][b_coords.col] = current_chunk->get_tile(chara->get_depth(), coords.row, coords.col);
        buffer[b_coords.row][b_coords.col]->visible = was_seen;
    }
}

void Game::drop_item(Item* item, Character* chara)
{
    IntPoint coords = IntPoint(chara->get_y(), chara->get_x());
    IntPoint chunk = IntPoint(chara->get_chunk_y(), chara->get_chunk_x());
    Chunk* current_chunk = chunk_map.get_chunk_abs(chunk.row, chunk.col);
    
    chara->drop_item(item);
    item->set_coords(coords);
    
    current_chunk->add_item(item, chara->get_depth());
    
    add_tile_to_buffer(chunk, coords, item->get_sprite());
}

/*========= PRIVATE METHODS ============*/

/*--------------------Base Model Functions-----------------*/

void Game::point_assertions(int row, int col) {
    assert(row >= 0);
    assert(row < SCREEN_HEIGHT);
    assert(col >= 0);
    assert(col < SCREEN_WIDTH);
}

void Game::set_tile(int row, int col, Tile* tile) {
    point_assertions(row, col);
    canvas[row][col] = tile;
}

void Game::set_tile(IntPoint point, Tile* tile) {
    point_assertions(point.row, point.col);
    canvas[point.row][point.col] = tile;
}

Item* Game::item_at_coords(IntPoint coords, IntPoint chunk, int depth) {

    vector<Item*>* item_list = chunk_map.get_chunk_abs(chunk.row,chunk.col)->get_items(depth);
    Item* temp_item = NULL;
    for(int i=0;i<item_list->size();i++) {

        if(item_list->at(i)->get_coords() == coords) {
            temp_item = item_list->at(i);
        }
    }
    return temp_item;
}

Tile* Game::get_tile(int row, int col) {
    point_assertions(row, col);
    return canvas[row][col];
}

Tile* Game::get_tile(IntPoint point) {
    point_assertions(point.row, point.col);
    return canvas[point.row][point.col];
}

bool Game::out_of_bounds(IntPoint point) {
    return out_of_bounds(point.row, point.col);
}

bool Game::is_paused() {
    return paused;
}

void Game::pause() {
    paused = true;
}

void Game::toggle_pause() {
    paused = !paused;
}


void Game::unpause() {
    paused = false;
}


/**
 * PRE: Will be given two ints representing a row and column.
 * POST: Returns true if the given point is out of bounds on the canvas, and
 * false otherwise.
 */
bool Game::out_of_bounds(int row, int col) {
    return (col < 0 || col >= GAME_WIDTH ||
            row < 0 || row >= GAME_HEIGHT);
}

/*
 * PRE: Takes an x and a y coordinate (chunk).
 * POST: Returns whether or not the chunk is currently in the buffer.
 */
bool Game::in_buffer(int row, int col) {
    return chunk_map.out_of_bounds(IntPoint(row, col));
}

bool Game::coords_in_buffer(int row, int col)
{
    bool is_in = row < buffer.size() && row >= 0 && col < buffer[row].size() && col >= 0;
    return is_in;
}

bool Game::in_range(IntPoint chunk, IntPoint coords, IntPoint range_chunk, IntPoint center, IntPoint radius) {
    IntPoint abs = get_abs(chunk, coords);
    IntPoint tl_abs = get_abs(range_chunk,
            IntPoint(center.row - radius.row,
                center.col - radius.col));

    IntPoint br_abs = get_abs(range_chunk,
            IntPoint(center.row + radius.row,
                center.col + radius.col));

    bool is_x = (abs.col>=tl_abs.col && abs.col<br_abs.col);
    bool is_y = (abs.row>=tl_abs.row && abs.row<br_abs.row);
    return (is_x && is_y);
}

IntPoint Game::get_canvas_coords(IntPoint chunk, IntPoint coords){
    IntPoint tl_canvas = get_abs(main_char.get_chunk(), IntPoint(main_char.get_y(), main_char.get_x()));
    IntPoint abs = get_abs(chunk, coords);
    return IntPoint(abs.row - tl_canvas.row, abs.col - tl_canvas.col);
}


IntPoint Game::get_buffer_coords(IntPoint chunk, IntPoint coords) {
    IntPoint tl_buffer = get_abs(IntPoint(main_char.get_chunk().row-1, main_char.get_chunk().col-1), IntPoint(0, 0));
    IntPoint abs = get_abs(chunk, coords);
    return IntPoint(abs.row - tl_buffer.row, abs.col - tl_buffer.col);
}


Character* Game::enemy_at_loc(IntPoint _chunk, IntPoint _coords) {
    IntPoint coords = get_buffer_coords(_chunk, _coords);
    return character_index[coords.row][coords.col];
}

/*----------------Rendering Functions----------------*/

//TODO Write PRE/POST for this function

/**
 * There are two ways that things get rendered.  The first is that
 * they get written to the buffer.  The second is that they get passed
 * into the rendering function.  Essentially: things that move should
 * get passed into gui_render, because they might change every time.
 * Things that don't move can be written to the buffer.
 */


void Game::show_chunk_objects() {
    std::vector<Spawner>* spawners;
    std::vector<Item*>* items;
    std::vector<Plant>* plants;
    
    Chunk* chunk;
    IntPoint chunk_coords;
    for(int i=main_char.get_chunk().row-1;i<=main_char.get_chunk().row+1;i++) {
        for(int j=main_char.get_chunk().col-1;j<=main_char.get_chunk().col+1;j++) {
            
            chunk = chunk_map.get_chunk_abs(IntPoint(i, j));
            IntPoint chunk_coords = IntPoint(i, j);
            
            if(chunk->get_depth()>main_char.get_depth()) {
                spawners = chunk->get_spawners(main_char.get_depth());
                items    = chunk->get_items(main_char.get_depth());
                plants   = chunk->get_plants(main_char.get_depth()); 
                
                plants_to_buffer(plants, chunk_coords);
                items_to_buffer(items, chunk_coords);
                spawners_to_buffer(spawners, chunk_coords);
            }
        }
    }
}

void Game::add_tile_to_buffer(IntPoint chunk, IntPoint coords, Tile* tile)
{
    IntPoint buffer_coords = get_buffer_coords(chunk, coords);
    if(coords_in_buffer(buffer_coords.row, buffer_coords.col))
    {
        buffer[buffer_coords.row][buffer_coords.col] = tile;
    }
}

bool Game::point_in_buffer(IntPoint chunk, IntPoint coords)
{
    IntPoint buffer_coords = get_buffer_coords(chunk, coords);
    return coords_in_buffer(buffer_coords.row, buffer_coords.col);
}

void Game::items_to_buffer(std::vector<Item*>* items, IntPoint chunk)
{
    for(int i=0;i<items->size();i++)
    {
        add_tile_to_buffer(chunk, items->at(i)->get_coords(), items->at(i)->get_sprite());
    }
}

void Game::spawners_to_buffer(std::vector<Spawner>* spawners, IntPoint chunk)
{
    for(int index=0;index<spawners->size();index++)
    {
        Spawner* spawner = &spawners->at(index);
        std::vector<Den> dens = spawner->get_spawn_points();
        for(int i=0;i<dens.size();i++)
        {
            Den* den = spawner->get_spawn_at(i);
            IntPoint coords = IntPoint(spawner->get_y() + den->get_y(), spawner->get_x() + den->get_x());
            den_to_buffer(den, chunk, coords);
        }
    }
}

void Game::plants_to_buffer(std::vector<Plant>* plants, IntPoint chunk)
{
    for(int i=0;i<plants->size();i++)
    {
        TileMatrix* plant_sprites = plants->at(i).get_sprites();
        for(int j=0;j<plant_sprites->size();j++)
        {
            for(int k=0;k<plant_sprites->at(j).size();k++)
            {
                IntPoint coords = plants->at(i).get_coords() + IntPoint(j, k);
                add_tile_to_buffer(chunk, coords, plants->at(i).get_sprite(j, k));
            }
        }
    }
}

void Game::den_to_buffer(Den* den, IntPoint chunk, IntPoint coords)
{
    for(int i=0;i<den->get_height();i++)
    {
        for(int j=0;j<den->get_width();j++)
        {
            if(den->tile_at(i, j) != tiledef::EMPTY)
            {
                IntPoint p_coords = coords + IntPoint(i - den->get_height()/2, j - den->get_width()/2);
                add_tile_to_buffer(chunk, p_coords, den->tile_pointer_at(i, j));
            }
        }
    }
}

/**
 * PRE: Will be given :int size:, the radius of the FOV circle
 * POST: Will set bresenham_lines, a vector of IntPoint vectors containing
 * points for raytraced lines.
 */
void Game::recalculate_visibility_lines(int size) {
    IntPoint true_center = IntPoint(0, 0);
    std::vector<IntPoint> circle_points = bresenham_circle(true_center, size);
    std::vector<IntPoint> smaller_circle = bresenham_circle(true_center, size - 1);

    for(int i = 0; i < smaller_circle.size(); i++) {
        circle_points.push_back(smaller_circle[i]);
    }

    std::vector<IntPoint> line_points;

    for(size_t i = 0; i < circle_points.size(); i++) {
        line_points = bresenham_line(true_center, circle_points[i]);
        bresenham_lines.push_back(line_points);
    }
}

/*
Here, we update the buffer based on the chunks surrounding the character's
current chunk.  This should be broken into several functions.  This will be
called whenever the character moves into a new chunk, so that the buffer
reflects the chunks surrounding the characters current one.
*/
void Game::update_buffer(IntPoint central_chunk) {
    int x, y;
    Chunk* current_chunk;
    Tile* buffer_tile;

    for(int row=central_chunk.row - 1;row<=central_chunk.row+1;row++) {

        for(int col=central_chunk.col-1;col<=central_chunk.col+1;col++) {
            x = col - (central_chunk.col - 1);
            y = row - (central_chunk.row - 1);
            //cout<<"Central chunk: "<<central_chunk.row<<" "<<central_chunk.col<<endl;
            current_chunk = chunk_map.get_chunk_abs(row, col);

            for (int a=0; a<CHUNK_HEIGHT; a++) {
                for (int b=0; b<CHUNK_WIDTH; b++) {
                    /**
                     *  This part is a bit confusing.  What I need is to write
                     *  the contents of the chunk to the appropriate place in
                     *  the buffer.  A and B represent the Y and X of
                     *  individual tiles.  So, for each chunk, the X and Y are
                     *  written to the buffer.  The chunks that we're iterating
                     *  through are essentially a 3x3 array.  Each chunk needs
                     *  to start being written at the appropriate location
                     *  (e.g. the second chunk needs to start where the first
                     *  one left off...), which is where the x and y variables
                     *  come in.  Starting at 0, the x and y are multiplied by
                     *  the width/height of the chunk, which is added to a and
                     *  b to get where the tile from the chunk corresponding to
                     *  a and b should be written to the buffer.  For example:
                     *  The first chunk should start writing tiles to the
                     *  buffer at 0,0.  The second chunk should start writing
                     *  tiles at 0 + CHUNK_WIDTH, and the third tile should
                     *  start writing at 0 + (CHUNK_WIDTH * 2).
                     */

                    int buffer_col = b + (x * CHUNK_WIDTH);
                    int buffer_row = a + (y * CHUNK_HEIGHT);

                    if(!current_chunk->out_of_bounds(main_char.get_depth(), a, b)) {
                        buffer_tile = current_chunk->get_tile(main_char.get_depth(), a, b);
                    } else {
                        buffer_tile = &tile_index[11];
                    }
                    buffer[buffer_row][buffer_col] = buffer_tile;
                }
            }
        }
    }
    show_chunk_objects();
    update_character_index();
    refresh();
}

void Game::update_character_index()
{
    clear_character_index();
    for(int i=0;i<character_list.size();i++)
    {
        if(point_in_buffer(character_list[i]->get_chunk(), character_list[i]->get_coords()))
        {
            character_to_index(character_list[i]);
        }
        /*
        else
        {
            remove_enemy(character_list[i]);
        }
        */
    }
    character_to_index(&main_char);
}

void Game::clear_character_index()
{
    for(int i=0;i<character_index.size();i++)
    {
        for(int j=0;j<character_index[i].size();j++)
        {
            character_index[i][j] = NULL;
        }
    }
}

void Game::character_to_index(Character* chara)
{
    IntPoint buffer_coords = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    character_index[buffer_coords.row][buffer_coords.col] = chara;
}

void Game::remove_index_char(Character* chara)
{
    IntPoint buffer_coords = get_buffer_coords(chara->get_chunk(), chara->get_coords());
    if(coords_in_buffer(buffer_coords.row, buffer_coords.col))
    {
        character_index[buffer_coords.row][buffer_coords.col] = NULL;
    }
}

void Game::update_chunk_map(IntPoint shift_dir) {
    chunk_map.shift_matrix(shift_dir, world_map);
}

/*
 * PRE: bresenham_lines should already be set by recalculate_visibility_lines().
 * POST: Draws a field-of-vision around the player - sets tiles' visibility
 * to true if they have been seen by the player.
 */
void Game::draw_visibility_lines() {
    IntPoint m_char = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 1; j < bresenham_lines[i].size(); j++) {
            current_point = bresenham_lines[i][j];
            row = current_point.row + m_char.row;
            col = current_point.col + m_char.col;

            if(!out_of_bounds(IntPoint(row, col))) {
                current_chunk_tile = get_tile(row, col);
                current_chunk_tile->visible = true;
                current_chunk_tile->seen = true;
                if(current_chunk_tile->opaque) {
                    break;
                }
            }
        }
    }
}

void Game::undo_visibility() {
    IntPoint m_char = IntPoint(GAME_HEIGHT/2, GAME_WIDTH/2);
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 1; j < bresenham_lines[i].size(); j++) {
            current_point = bresenham_lines[i][j];
            row = current_point.row + m_char.row;
            col = current_point.col + m_char.col;

            if(!out_of_bounds(IntPoint(row, col))) {
                current_chunk_tile = get_tile(row, col);
                current_chunk_tile->visible = false;
                current_chunk_tile->seen = true;
                /* *current_chunk_tile = ROOM_WALL;
                 *current_chunk_tile->visible = true;
                 *current_chunk_tile->can_be_moved_through = true;
                 */
                if(current_chunk_tile->opaque) {
                    break;
                }
            }
        }
    }
}


/*-----------------------------------DEBUG FUNCTIONS----------------------*/


void Game::spawn_enemy(int chunk_x, int chunk_y, int x, int y, int depth, int type) {
        Enemy* temp = new Enemy(x, y, depth, ENEMY_LIST[type]);
        temp->set_chunk(IntPoint(chunk_y, chunk_x));
        character_queue.push_back(temp);
        character_list.push_back(temp);
        character_to_index(temp);
}

void Game::teleport(int chunk_x, int chunk_y, int x, int y)
{
    main_char.set_x(x);
    main_char.set_y(y);
    main_char.set_chunk(IntPoint(chunk_y, chunk_x));
    update_buffer(main_char.get_chunk());
    unsigned int delta_x = chunk_x - main_char.get_chunk().col;
    unsigned int delta_y = chunk_y - main_char.get_chunk().row;
    if(delta_x > 1 || delta_y > 1)
    {
        chunk_map.serialize_all();
        chunk_map.populate_initial(main_char.get_chunk(), world_map);
    }
    else
    {
        update_chunk_map(IntPoint(delta_y, delta_x));
    }

}

/*-----------------------ANIMATION FUNCTIONS---------------------------*/

void Game::create_explosion(int x, int y, int chunk_x, int chunk_y)
{
    anim_queue.push_back(construct_explosion(x, y, chunk_x, chunk_y, 5, YELLOW));
}
