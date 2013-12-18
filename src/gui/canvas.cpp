#include  "canvas.h"
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


Canvas::Canvas() {
    STARTING_WIDTH = 100;
    STARTING_HEIGHT = 50;
    chunk_map = ChunkMatrix(10, vector<Chunk>(10)); 
    //Give me a buffer size of 150x300 (tiles, which are 8x16 pixels)
    //The buffer is what the screen draws from.
    buffer = TileMatrix(150, vector<Tile>(300));
    
    //Each chunk holds an overworld and several
    //dungeons, which are generated upon chunk creation.
    //This is the "starting" chunk (arbitrary).
    chunk_map[5][8] = Chunk(5, 8, STARTING_WIDTH, STARTING_HEIGHT);
    main_char = Main_Character(101, 50, 25, 3, chunk_map[5][8], -1);
    
    //What gets drawn to the screen
    canvas = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH));
    update_buffer();
    cout<<"buffer updated."<<endl;
}

void Canvas::set_tile(int row, int col, Tile tile) {
    assert(row >= 0);
    assert(row < STARTING_HEIGHT);
    assert(col >= 0);
    assert(col < STARTING_WIDTH);
    canvas[row][col] = tile;
}

/*
This is to refresh the screen whenever the character moves.
*/
void Canvas::refresh() {
    //If the character has gone out of bounds of the chunk,t hen the chunk and
    //buffer need updated
    if(out_of_bounds()) {
        update_chunk();
        update_buffer();
    }
    //This section checks if the character is in a dungeon, or the overworld. In
    //the dungeon, the character moves around the screen.  In the overworld, a
    //calculation is done to keep the character at the center of the screen.
    //This is where the canvas is written to.
    if(main_char.get_depth() >=0) {
        for(int i = 0; i < STARTING_HEIGHT; i++) {
            for(int j = 0; j < STARTING_WIDTH; j++) {
                Chunk* current_chunk =
                    &chunk_map[main_char.get_chunk_y()][main_char.get_chunk_x()];
                canvas[i][j] = current_chunk->get_tile(main_char.get_depth(),i,j);
            }
        }
        canvas[main_char.get_y_loc()][main_char.get_x_loc()] = MAIN_CHAR;
    } else {
        //STARTING_HEIGHT is actually screen height, same for STARTING_WIDTH
        for(int i = 0; i < STARTING_HEIGHT; i++) { 
            for (int j = 0; j < STARTING_WIDTH; j++) {
                int buffer_tile_row = (STARTING_HEIGHT + main_char.get_y_loc()) -
                    (STARTING_HEIGHT/2) + i; 
                int buffer_tile_col = (STARTING_WIDTH + main_char.get_x_loc()) -
                    (STARTING_WIDTH/2) + j; 
                canvas[i][j] = buffer[buffer_tile_row][buffer_tile_col];
            }
        }
        canvas[STARTING_HEIGHT/2][STARTING_WIDTH/2] = MAIN_CHAR;
    }

    //draw_visibility_lines();
}

void Canvas::draw_visibility_lines() {
    IntPoint point1 = IntPoint(main_char.get_y_loc() + 10, 
                               main_char.get_x_loc() + 8);
    IntPoint character_loc = IntPoint(main_char.get_y_loc(),
                                      main_char.get_x_loc());
    std::vector<IntPoint> points = bresenham_line(character_loc, point1);
    IntPoint current_point;
    for(size_t i = 0; i < points.size(); i++){
        current_point = points[i];
        canvas[current_point.row][current_point.col] = WALL;
    }

}

bool Canvas::out_of_bounds() {
    return (main_char.get_x_loc() < 0 || 
            main_char.get_x_loc() >= STARTING_WIDTH || 
            main_char.get_y_loc() < 0 || 
            main_char.get_y_loc() >= STARTING_HEIGHT);
}

void Canvas::update_chunk() {
    int x = main_char.get_chunk_x();
    int y = main_char.get_chunk_y();

    
    if (main_char.get_x_loc() < 0 ) {
        x -= 1;
        main_char.set_x(STARTING_WIDTH-1);
    } else if (main_char.get_x_loc() >= STARTING_WIDTH) {
        x += 1;
        main_char.set_x(0);
    }
    
    if(main_char.get_y_loc() < 0) {
        y -= 1;
        main_char.set_y(STARTING_HEIGHT-1);
    } else if (main_char.get_y_loc()>= STARTING_HEIGHT) {
        y += 1;
        main_char.set_y(0);
    }
    main_char.update_dungeon(chunk_map[y][x]);
}


/*
This function is an abomination, a sin against code.

SACERDOS ab Ordinario delegatus, rite confessus, aut saltem corde peccata sua
detestans, peracto, si commode fieri possit, Sanctissimo Missæ sacrificio,
divinoque auxilio piis precibus implorato, superpelliceo et stola violacea
indutus, et coram se habens obsessum ligatum, si sit periculum, eum, se et
astantes communiat signo crucis, et aspergat aqua benedicta, et genibus flexis,
aliis respondentibus, dicat Litanias ordinarias usque ad Preces exclusive.

Here, we update the buffer based on the chunks surrounding the character's
current chunk.  This should be broken into several functions.  This will be
called whenever the character moves into a new chunk, so that the buffer
reflects the chunks surrounding the characters current one.
*/
void Canvas::update_buffer() {
    int x = 0;
    int y = 0;
   
    //loop through the characters y chunk coordinate, +/- 1.  If the character
    //was in chunk 7, this would loops through 6, 7, 8. 
    for(int col=main_char.get_chunk_y() - 1;col<=main_char.get_chunk_y()+1;col++) {
        //Check to ensure that the chunk map is big enough.
        if(chunk_map.size() < (size_t) col + 1) {
            chunk_map.resize(col + 1);
        }

        //as above, but with the x coordinate.
        for(int row=main_char.get_chunk_x()-1;row<=main_char.get_chunk_x()+1;row++) {
            
            if (chunk_map[col].size() < (size_t) row + 1) {
                chunk_map[col].resize(row + 1);
            }
            
            //check to ensure that the chunk we're about to operate on is
            //initialized.  If not, initialize it.
            if (chunk_map[col][row].is_initialized() == false) {
                std::cout<<"initializing with "<<col<<" & "<<row<<std::endl;
                chunk_map[col][row] = Chunk(col, row, STARTING_WIDTH, STARTING_HEIGHT);
            }
            
            for (int a=0;a<STARTING_HEIGHT;a++) {
                for (int b=0;b<STARTING_WIDTH;b++) {
                    /*
                        This part is a bit confusing.  What I need is to write
                        the contents of the chunkt to the appropriate place in
                        the buffer.  A and B represent the Y and X of
                        individual tiles.  So, for each chunk, the X and Y are
                        written to the buffer.  The chunks that we're iterating
                        through are essentially a 3x3 array.  Each chunk needs
                        to start being written at the appropriate location
                        (e.g. the second chunk needs to start where the first
                        one left off...), which is where the x and y variables
                        come in.  Starting at 0, the x and y are multiplied by
                        the width/height of the chunk, which is added to a and
                        b to get where the tile from the chunk corresponding to
                        a and b should be written to the buffer.  For example:
                        The first chunk should start writing tiles to the
                        buffer at 0,0.  The second chunk should start writing
                        tiles at 0 + CHUNK_WIDTH, and the third tile should
                        start writing at 0 + (CHUNK_WIDTH * 2).
                    */


                    // These two things are always true. Setting them here to
                    // avoid awkward incrementing. I always think it's a bad
                    // idea to do incrementing (y++, x++) after every run of a
                    // for-loop, because of the redundancy.
                    x = col - (main_char.get_chunk_y() - 1);
                    y = row - (main_char.get_chunk_x() - 1);

                    int buffer_row = a + (x * STARTING_HEIGHT);
                    int buffer_col = b + (y * STARTING_WIDTH);
                    Tile buffer_tile = chunk_map[col][row].get_tile(-1, a, b);

                    buffer[buffer_row][buffer_col] = buffer_tile;
                }
            }
        }
    }
}

const Chunk& Canvas::get_chunk() {
    return chunk_map[main_char.get_chunk_x()][main_char.get_chunk_y()];
}

//Since this is a const reference, will we have to call it
//more than once? Maybe not...
const std::vector<std::vector<Tile> >& Canvas::get_matrix() {
    return canvas;
}
