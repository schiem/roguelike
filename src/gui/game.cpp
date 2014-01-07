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

#include  <game.h>
#include <iostream>
using namespace std;
using namespace tiledef;
using namespace enemies;
using namespace map_tile;
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
223

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
Game::Game(int w, int h) {
    STARTING_WIDTH = w;
    STARTING_HEIGHT = h;
    initialized = false;
    enemy_list = std::vector<Enemy>();
}

void Game::init(const MapTileMatrix& _world_map, IntPoint selected_chunk) {
    world_map = _world_map;
    chunk_map = ChunkMatrix(world_map.size(), vector<Chunk>(world_map[0].size()));
    //Give me a buffer size of 150x300 (tiles, which are 8x16 pixels)
    //The buffer is what the screen draws from.
    buffer = TilePointerMatrix(150, vector<Tile*>(300));


    //Each chunk holds an overworld and several
    //dungeons, which are generated upon chunk creation.
    //This is the "starting" chunk (arbitrary).
    chunk_map[selected_chunk.row][selected_chunk.col] = Chunk(STARTING_WIDTH, STARTING_HEIGHT,
                           world_map[selected_chunk.row][selected_chunk.col],
                           selected_chunk.row, selected_chunk.col);

    main_char = Main_Character(101, 50, 25, MAIN_CHAR, selected_chunk.col, selected_chunk.row, -1);

    //What gets drawn to the screen
    canvas = TilePointerMatrix(STARTING_HEIGHT, vector<Tile*>(STARTING_WIDTH));
    update_chunk_map(main_char.get_chunk());
    update_buffer(main_char.get_chunk());
    recalculate_visibility_lines(15);
    refresh();

    //TODO THIS IS A TEMPORARY HACK THAT SHOULD BE GONE BEFORE I GO TO TANZANIA,
    //SO HELP ME GOD.
    block_wall_tile = BLOCK_WALL;

    initialized = true;
}

Chunk* Game::get_current_chunk() {
    return &chunk_map[main_char.get_chunk().row][main_char.get_chunk().col];
}


bool Game::is_initialized() {
    return initialized;
}

void Game::act(long delta_ms) {
    run_enemies(delta_ms);
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
    IntPoint tl_abs = get_abs(main_char.get_chunk(), IntPoint(main_char.get_y() - STARTING_HEIGHT/2, main_char.get_x() - STARTING_WIDTH/2));
    temp = IntPoint(abs.row - tl_abs.row, abs.col - tl_abs.col);
    return temp;
}

std::vector<Enemy> Game::get_vis_enemies() {
    std::vector<Enemy> temp = std::vector<Enemy>();
    for(int i=0;i<enemy_list.size();i++)
    {
        IntPoint chunk = IntPoint(enemy_list[i].get_chunk_y(), enemy_list[i].get_chunk_x());
        IntPoint coords = IntPoint(enemy_list[i].get_y(), enemy_list[i].get_x());
        IntPoint main_char_coords = IntPoint(main_char.get_y(), main_char.get_x());
        IntPoint radius  = IntPoint(STARTING_HEIGHT/2, STARTING_WIDTH/2);
        if(in_range(chunk, coords, main_char.get_chunk(), main_char_coords, radius) && enemy_list[i].get_depth() == main_char.get_depth())
        {
            temp.push_back(enemy_list[i]);
        }
    }
    return temp;
}


/*
 * Resets the top layer.
 * Checks to see if the character is out of the chunk.  If so, update the chunk/chunk_map
 * Updates the canvas with the area around the character in terms of buffer coordinates.
 * Draws visibility lines.
 * This is to refresh the screen whenever the character moves.
 */
void Game::refresh() {
    for(int i = 0; i < STARTING_HEIGHT; i++) {
        for (int j = 0; j < STARTING_WIDTH; j++) {
            int buffer_tile_row = (STARTING_HEIGHT + main_char.get_y()) -
                (STARTING_HEIGHT/2) + i;
            int buffer_tile_col = (STARTING_WIDTH + main_char.get_x()) -
                (STARTING_WIDTH/2) + j;
            set_tile(i, j, buffer[buffer_tile_row][buffer_tile_col]);
        }
    }
    draw_visibility_lines();
}

/*---------------------Enemy Helper Functions---------------------*/
/* PRE: None
 * POST: Iterates through the chunks which are part of the buffer and runs any
 * spawner (TODO: make it so it will run any spawners, plural).  If the spawner
 * returns that it should spawn, then it will spawn an enemy based on the type
 * of spawner that it is and append it to the enemy list.
 */
void Game::run_spawners() {
    Spawner spawner;
    Chunk* chunk;
    IntPoint chunk_coords;
    for(int i=main_char.get_chunk().row-1;i<main_char.get_chunk().row+1;i++) {
        for(int j=main_char.get_chunk().col-1;j<main_char.get_chunk().col+1;j++) {
            chunk = &chunk_map[i][j];
            if(chunk->get_depth()>main_char.get_depth() && chunk->get_type() == MAP_DEFAULT)
            {
                spawner = chunk->get_spawner(main_char.get_depth());
                if(spawner.should_spawn()) {
                    enemy_list.push_back(spawner.spawn_creep(j, i));
                }
            }
        }
    }
}
/* PRE: None
 * POST: Iterates through the enemy list.  For each enemy, it checks to see if
 * it is in the current buffer.  If not, it deletes it.  It then checks to see if
 * it is at the current depth.  If not, it does nothing.
 */
void Game::run_enemies(long delta_ms) {
    Enemy* enemy;
    for(int i=0;i<enemy_list.size();i++) {
        enemy = &enemy_list[i];
        IntPoint enem_chunk = IntPoint(enemy->get_chunk_y(), enemy->get_chunk_x());
        IntPoint enem_coords = IntPoint(enemy->get_y(), enemy->get_x());

        if(!in_buffer(enemy->get_chunk_x(), enemy->get_chunk_y())) {
            enemy_list.erase(enemy_list.begin() + i);

        } else if(enemy->get_depth() == main_char.get_depth()) {
            TileMatrix surroundings = get_surroundings(enem_chunk, enem_coords, enemy->get_depth(),
                    IntPoint(enemy->get_sight(), enemy->get_sight()));

            IntPoint main_char_point(main_char.get_y(), main_char.get_x());

            if(in_range(enem_chunk, enem_coords, main_char.get_chunk(), 
                        main_char_point, IntPoint(enemy->get_sight(), enemy->get_sight()))) {

                enemy->run_ai(surroundings, &main_char, delta_ms);
            } else {
                enemy->run_ai(surroundings, NULL, delta_ms);
            }
        }
    }
}

/*--------------------Main Char Functions----------------------*/
void Game::change_main_depth(int direction) {
    assert(direction == -1 || direction == 1);
    Chunk * current_chunk;
    current_chunk = get_current_chunk();
    Tile* current_tile = current_chunk->get_tile(main_char.get_depth(),
            main_char.get_y(), main_char.get_x());

    if(direction == -1) {
        if (main_char.get_depth()-1 >= -1) {
            if(*current_tile == UP_STAIR) {
                main_char.set_depth(main_char.get_depth() - 1);
                main_char.set_x(current_chunk->get_down_stair(main_char.get_depth()).col);
                main_char.set_y(current_chunk->get_down_stair(main_char.get_depth()).row);
            }
        }
    } else {
        if (main_char.get_depth()+1 < current_chunk->get_depth()) {
            if(*current_tile == DOWN_STAIR) {
                main_char.set_depth(main_char.get_depth() + 1);
                main_char.set_x(current_chunk->get_up_stair(main_char.get_depth()).col);
                main_char.set_y(current_chunk->get_up_stair(main_char.get_depth()).row);
            }
        }
    }

    //Dungeon FOV is shorter than overworld FOV
    update_buffer(main_char.get_chunk());
    if(main_char.get_depth() >= 0) {
        recalculate_visibility_lines(10);
    }
}

void Game::move_main_char(int col_change, int row_change) {
    undo_visibility();

    int row = main_char.get_y();
    int col = main_char.get_x();
    int next_col = main_char.get_x() + col_change;
    int next_row = main_char.get_y() + row_change;


    IntPoint new_chunk = IntPoint(main_char.get_chunk_y() + (next_row>=STARTING_HEIGHT) - (next_row<0), main_char.get_chunk_x() + (next_col>=STARTING_WIDTH) - (next_col<0));

    next_col = next_col +  (STARTING_WIDTH * (next_col<0)) - (STARTING_WIDTH * (next_col>=STARTING_WIDTH));
    next_row = next_row +  (STARTING_HEIGHT * (next_row<0)) - (STARTING_HEIGHT * (next_row>=STARTING_HEIGHT));

    if(chunk_map[new_chunk.row][new_chunk.col].get_tile(main_char.get_depth(), next_row, next_col)->can_be_moved_through)
    {
        col = next_col;
        row = next_row;
        main_char.set_x(col);
        main_char.set_y(row);
        if(main_char.get_chunk() != new_chunk)
        {
            main_char.set_chunk(new_chunk);
            update_chunk_map(main_char.get_chunk());
            cout<<main_char.get_chunk()<<endl;
            update_buffer(main_char.get_chunk());

        }
    }


}


/*========= PRIVATE METHODS ============*/

/*--------------------Base Model Functions-----------------*/
/*
 * PRE: Will be given two integers representing a row and column.
 * POST: Will run the point represented by this row and column through a series
 * of assertions to ensure that it will not produce a segfault.
 */
void Game::point_assertions(int row, int col) {
    assert(row >= 0);
    assert(row < STARTING_HEIGHT);
    assert(col >= 0);
    assert(col < STARTING_WIDTH);
}

/**
 * PRE: Will be given two ints representing a row and column, and a Tile.
 * POST: Sets the tile at the given point on the canvas to the given tile,
 * using assertions in point_assertions.
 */
void Game::set_tile(int row, int col, Tile* tile) {
    point_assertions(row, col);
    canvas[row][col] = tile;
}

/**
 * PRE: Will be given an IntPoint and a Tile.
 * POST: Sets the tile at the given point on the canvas to the given tile,
 * using assertions in point_assertions.
 */
void Game::set_tile(IntPoint point, Tile* tile) {
    point_assertions(point.row, point.col);
    canvas[point.row][point.col] = tile;
}

/**
 * PRE: Will be given two integers representing a row and a column.
 * POST: Returns a pointer to the tile on the canvas at that row and column,
 * using assertions in point_assertions.
 */
Tile* Game::get_tile(int row, int col) {
    point_assertions(row, col);
    return canvas[row][col];
}

/**
 * PRE: Will be given an IntPoint.
 * POST: Returns a pointer to the tile on the canvas at that point, using
 * assertions in point_assertions.
 */
Tile* Game::get_tile(IntPoint point) {
    point_assertions(point.row, point.col);
    return canvas[point.row][point.col];
}

/**
 * PRE: Will be given an IntPoint.
 * POST: Returns true if the given point is out of bounds on the canvas, and
 * false otherwise.
 */
bool Game::out_of_bounds(IntPoint point) {
    return (point.col < 0 || point.col >= STARTING_WIDTH ||
            point.row < 0 || point.row >= STARTING_HEIGHT);
}

/**
 * PRE: Will be given two ints representing a row and column.
 * POST: Returns true if the given point is out of bounds on the canvas, and
 * false otherwise.
 */
bool Game::out_of_bounds(int row, int col) {
    return (col < 0 || col >= STARTING_WIDTH ||
            row < 0 || row >= STARTING_HEIGHT);
}

/*
 * PRE: Takes an x and a y coordinate (chunk).
 * POST: Returns whether or not the chunk is currently in the buffer.
 */
bool Game::in_buffer(int x, int y) {
    bool is_x = (x>=main_char.get_chunk().col-1 && x<=main_char.get_chunk().col+1);
    bool is_y = (y>=main_char.get_chunk().row-1 && y<=main_char.get_chunk().row+1);
    return (is_x && is_y);
}

/*
 * PRE: Takes an IntPoint representing chunk coordinates, and an IntPoint
 * representing coordinates within that chunk.
 * POST: Returns whether or not that chunk is within the visible range of the
 * character (if it is within the canvas).
 */
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


IntPoint Game::get_buffer_coords(IntPoint chunk, IntPoint coords) {
    IntPoint tl_buffer = get_abs(IntPoint(main_char.get_chunk().row-1, main_char.get_chunk().col-1), IntPoint(0, 0));
    IntPoint abs = get_abs(chunk, coords);
    return IntPoint(abs.row - tl_buffer.row, abs.col - tl_buffer.col);
}

/* PRE:  Takes in an IntPoint representing chunk coordinates, an InPoint
 * representing coordinates within that chunk, and a depth.
 * POST: Returns a TileMatrix containing the area (20x20) surrounding
 * the coordinates given.
 */
Game::TileMatrix Game::get_surroundings(IntPoint _chunk, IntPoint _coords, int depth, IntPoint radius) {
    radius = IntPoint(radius.row+1, radius.col+1);
    TileMatrix surroundings = TileMatrix(radius.row * 2, std::vector<Tile>(radius.col * 2));
    Tile new_tile;
    IntPoint buffer_coords = get_buffer_coords(_chunk, _coords);
    IntPoint sur_coords;
    for(int row=(buffer_coords.row-radius.row);row<(buffer_coords.row+radius.row);row++) {
        for(int col=(buffer_coords.col-radius.col);col<(buffer_coords.col+radius.col);col++) {
            sur_coords = IntPoint(row+radius.row - buffer_coords.row, col+radius.col - buffer_coords.col);
            if(row<0 || row>= STARTING_HEIGHT * 3 || col<0 || col>= STARTING_WIDTH * 3) {
                new_tile = EMPTY;
            } else if (sur_coords.row == 0 || sur_coords.col == 0 || sur_coords.row == (radius.row*2-1) || sur_coords.col == radius.col*2 - 1)
            {
                new_tile = BLOCK_WALL;
            }
            else
            {
                new_tile = *buffer[row][col];
            }
            surroundings[sur_coords.row][sur_coords.col] = new_tile;
        }
    }

    return surroundings;
}

/*----------------Rendering Functions----------------*/

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

    for(int row=central_chunk.row - 1;row<=central_chunk.row+1;row++) {

        for(int col=central_chunk.col-1;col<=central_chunk.col+1;col++) {

            for (int a=0;a<STARTING_HEIGHT;a++) {
                for (int b=0;b<STARTING_WIDTH;b++) {
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

                    x = col - (central_chunk.col - 1);
                    y = row - (central_chunk.row - 1);
                    int buffer_col = b + (x * STARTING_WIDTH);
                    int buffer_row = a + (y * STARTING_HEIGHT);

                    Tile* buffer_tile;
                    if(!chunk_map[row][col].out_of_bounds(main_char.get_depth(), a, b)) {
                        buffer_tile = chunk_map[row][col].get_tile(main_char.get_depth(), a, b);
                    } else {
                        buffer_tile = &block_wall_tile;
                    }


                    buffer[buffer_row][buffer_col] = buffer_tile;
                }
            }
        }
    }
}

void Game::update_chunk_map(IntPoint central_chunk) {
    for(int row=central_chunk.row - 1;row<=central_chunk.row+1;row++) {
        //Check to ensure that the chunk map is big enough.
        if(chunk_map.size() < (size_t) row + 1) {
            chunk_map.resize(row + 1);
        }

        //as above but with the x coordinate.
        for(int col=central_chunk.col-1;col<=central_chunk.col+1;col++) {
            if (chunk_map[row].size() < (size_t) col + 1) {
                chunk_map[row].resize(col + 1);
            }

            //check to ensure that the chunk we're about to operate on is
            //initialized.  If not, initialize it.
            if (chunk_map[row][col].is_initialized() == false) {
                chunk_map[row][col] = Chunk(STARTING_WIDTH, STARTING_HEIGHT,
                                             world_map[row][col], row, col);
                chunk_map[row][col].serialize(row, col);
            }
        }
    }
}

/*
 * PRE: bresenham_lines should already be set by recalculate_visibility_lines().
 * POST: Draws a field-of-vision around the player - sets tiles' visibility
 * to true if they have been seen by the player.
 */
void Game::draw_visibility_lines() {
    /*
    IntPoint m_char;
    if(main_char.get_depth() >=0) {
        m_char = IntPoint(main_char.get_y(),
                                      main_char.get_x());
    } else {
    */
    IntPoint m_char = IntPoint(STARTING_HEIGHT/2, STARTING_WIDTH/2);
    //}
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 0; j < bresenham_lines[i].size(); j++) {
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
    /*
    IntPoint m_char;
    if(main_char.get_depth() >=0) {
        m_char = IntPoint(main_char.get_y(),
                                      main_char.get_x());
    } else {
    */
    IntPoint m_char = IntPoint(STARTING_HEIGHT/2, STARTING_WIDTH/2);
    //}
    Tile* current_chunk_tile;
    IntPoint current_point;
    int row, col;

    for(size_t i = 0; i < bresenham_lines.size(); i++) {
        for(size_t j = 0; j < bresenham_lines[i].size(); j++) {
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
