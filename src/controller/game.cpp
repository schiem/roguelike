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
 //umm...this operates on two different coordinate systems, fyi.  It will
 //almost never work -MJY 2/2/15
bool Game::chunk_in_buffer(int row, int col) {
    IntPoint center = chunk_map.get_center();
    int offset = (chunk_map.get_diameter() - 1)/2;
    IntPoint buffer_chunk = IntPoint(center.row - offset - row, center.col - offset - col); 
    return chunk_map.out_of_bounds(buffer_chunk);
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
