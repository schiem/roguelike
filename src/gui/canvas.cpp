#include "canvas.h"

using namespace std;
using namespace tiledef;

Canvas::Canvas()
{
	chunk_map = ChunkMatrix(10, vector<Chunk>(10));	
	buffer = TileMatrix(300, vector<Tile>(150));
	chunk = Chunk(0, 0, 100, 50);
    main_char = Main_Character(100, 1, 1, 3, &chunk, -1);

    canvas = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH));
}

void Canvas::refresh()
{
    
	for(int i = 0; i < chunk.height; i++) {
        for(int j = 0; j < chunk.width; j++) {
            canvas[i][j] = chunk.get_tile(main_char.get_depth(),i, j);
        }
    }
    canvas[main_char.get_y_loc()][main_char.get_x_loc()] = MAIN_CHAR;
}

const Chunk& Canvas::get_chunk()
{
    return chunk;
}

//Since this is a const reference, will we have to call it
//more than once? Maybe not...
const std::vector<std::vector<Tile> >& Canvas::get_matrix()
{
    return canvas;
}
