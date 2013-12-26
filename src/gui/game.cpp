#include  "game.h"
#include <iostream>
using namespace std;
using namespace tiledef;

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


Game::Game() {
    STARTING_WIDTH = 100;
    STARTING_HEIGHT = 50;
    chunk_map = ChunkMatrix(10, vector<Chunk>(10)); 
    //Give me a buffer size of 150x300 (tiles, which are 8x16 pixels)
    //The buffer is what the screen draws from.
    buffer = TilePointerMatrix(150, vector<Tile*>(300));

    //Each chunk holds an overworld and several
    //dungeons, which are generated upon chunk creation.
    //This is the "starting" chunk (arbitrary).
    chunk_map[5][8] = Chunk(STARTING_WIDTH, STARTING_HEIGHT);


    top_layer = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH, EMPTY));
    main_char = Main_Character(101, 50, 25, MAIN_CHAR, -1);
    main_char_chunk.row = 5;
    main_char_chunk.col = 8;

    //What gets drawn to the screen
    canvas = TilePointerMatrix(STARTING_HEIGHT, vector<Tile*>(STARTING_WIDTH));
    update_chunk_map(main_char_chunk);
    update_buffer(main_char_chunk);

    recalculate_visibility_lines(15);
    refresh();
}

/**
 * PRE: Will be given :int size:, the radius of the FOV circle
 * POST: Will set bresenham_lines, a vector of IntPoint vectors containing
 * points for raytraced lines.
 */
void Game::recalculate_visibility_lines(int size) {
    IntPoint true_center = IntPoint(0, 0);
    std::vector<IntPoint> circle_points = bresenham_circle(true_center, size);
    std::vector<IntPoint> line_points;

    for(size_t i = 0; i < circle_points.size(); i++) {
        line_points = bresenham_line(true_center, circle_points[i]);
        bresenham_lines.push_back(line_points);
    }
}

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

/**
 *This is to refresh the screen whenever the character moves.
 */
void Game::refresh() {
    //Reset the top layer
    top_layer = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH, EMPTY));
    //If the character has gone out of bounds of the chunk,t hen the chunk and
    //buffer need to be updated
    if(out_of_bounds(main_char.get_y(), main_char.get_x())) {
        update_chunk();
        update_chunk_map(main_char_chunk);
        cout<<main_char_chunk<<endl;
        update_buffer(main_char_chunk);
    }
    //This section checks if the character is in a dungeon, or the overworld. In
    //the dungeon, the character moves around the screen.  In the overworld, a
    //calculation is done to keep the character at the center of the screen.
    //This is where the canvas is written to.
    /*
    if(main_char.get_depth() >=0) {
        for(int i = 0; i < STARTING_HEIGHT; i++) {
            for(int j = 0; j < STARTING_WIDTH; j++) {
                Chunk* current_chunk =
                    &chunk_map[main_char_chunk.row][main_char_chunk.col];
                set_tile(i, j, current_chunk->get_tile(main_char.get_depth(),i,j));
            }
        top_layer[main_char.get_y()][main_char.get_x()] = main_char.get_char();
        }
    } else {
        */
        for(int i = 0; i < STARTING_HEIGHT; i++) {
            for (int j = 0; j < STARTING_WIDTH; j++) {
                int buffer_tile_row = (STARTING_HEIGHT + main_char.get_y()) -
                    (STARTING_HEIGHT/2) + i;
                int buffer_tile_col = (STARTING_WIDTH + main_char.get_x()) -
                    (STARTING_WIDTH/2) + j;
                set_tile(i, j, buffer[buffer_tile_row][buffer_tile_col]);
            }
        }
        top_layer[STARTING_HEIGHT/2][STARTING_WIDTH/2] = main_char.get_char();
    //}
    draw_visibility_lines();
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


/*
 * PRE: TODO
 * POST: TODO
 */
void Game::update_chunk() {
    int mc_row = main_char.get_y();
    int mc_col = main_char.get_x();
    if (mc_col < 0 ) {
        main_char_chunk.col -= 1;
        main_char.set_x(STARTING_WIDTH-1);
    } else if (mc_col >= STARTING_WIDTH) {
        main_char_chunk.col += 1;
        main_char.set_x(0);
    }

    if(mc_row < 0) {
        main_char_chunk.row -= 1;
        main_char.set_y(STARTING_HEIGHT-1);
    } else if (mc_row >= STARTING_HEIGHT) {
        main_char_chunk.row += 1;
        main_char.set_y(0);
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
                chunk_map[row][col] = Chunk(STARTING_WIDTH, STARTING_HEIGHT);
            }
        }
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
                    Tile* buffer_tile = chunk_map[row][col].get_tile(main_char.get_depth(), a, b);
                    buffer[buffer_row][buffer_col] = buffer_tile;
                }
            }
        }
    }
}

Chunk* Game::get_chunk() {
    return &chunk_map[main_char_chunk.row][main_char_chunk.col];
}

//Since this is a const reference, will we have to call
//more than once? Maybe not...
const std::vector<std::vector<Tile*> >& Game::get_matrix() {
    return canvas;
}

const std::vector<std::vector<Tile> > Game::get_top_layer(){
    return top_layer;
}


/*--------------------Main Char Functions----------------------*/
void Game::change_main_depth(int direction) {
    assert(direction == -1 || direction == 1);
    Chunk * current_chunk;
    current_chunk = get_chunk();
    Tile* current_tile = current_chunk->get_tile(main_char.get_depth(),
            main_char.get_y(), main_char.get_x());

    if(direction == -1) {
        if (main_char.get_depth()-1 >= -1) {
            if(*current_tile == UP_STAIR) {
                main_char.set_depth(main_char.get_depth() - 1);
                main_char.set_x(current_chunk->get_down_stair(main_char.get_depth())[0]);
                main_char.set_y(current_chunk->get_down_stair(main_char.get_depth())[1]);
            }
        }
    } else {
        if (main_char.get_depth()+1 < current_chunk->get_depth()) {
            if(*current_tile == DOWN_STAIR) {
                main_char.set_depth(main_char.get_depth() + 1);
                main_char.set_x(current_chunk->get_up_stair(main_char.get_depth())[0]);
                main_char.set_y(current_chunk->get_up_stair(main_char.get_depth())[1]);
            }
        }
    }

    //Dungeon FOV is shorter than overworld FOV
    update_buffer(main_char_chunk);
    if(main_char.get_depth() >= 0) {
        recalculate_visibility_lines(10);
    }
}

void Game::move_main_char(int col_change, int row_change) {
    undo_visibility();
    int row = main_char.get_y();
    int col = main_char.get_x();
    int next_col = col + col_change;
    int next_row = row + row_change;
    if((next_col < 0) ||  (next_col >= get_chunk()->width) ||
            (next_row < 0) ||  (next_row >= get_chunk()->height)) {
        col += col_change;
        row += row_change;
    } else if (get_chunk()->get_tile(main_char.get_depth(),
                next_row, next_col)->can_be_moved_through) {
        col += col_change;
        row += row_change;
    }
    main_char.set_x(col);
    main_char.set_y(row);

}
