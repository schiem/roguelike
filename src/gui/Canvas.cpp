#include "Canvas.h"

using namespace std;
typedef std::vector<std::vector<Tile> > TileMatrix;

Canvas::Canvas()
{
    db = DungeonBuilder(80, 40, 10);
    /*
    db.build_pblind_dungeon(5, 1, 1);
    dungeon = db.get_dungeon();

    main_char = Main_Character(100, 1, 1, 3);

    canvas = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH));
    */
}

/*
void Canvas::refresh()
{
    for(int i = 0; i < dungeon.height; i++) {
        for(int j = 0; j < dungeon.width; j++) {
            canvas[i][j] = dungeon.get_tile(i, j);
        }
    }
    canvas[main_char.get_y_loc()][main_char.get_x_loc()] = MAIN_CHAR;
}

const Dungeon& Canvas::get_dungeon()
{
    return dungeon;
}

//Since this is a const reference, will we have to call it
//more than once? Maybe not...
const TileMatrix& Canvas::get_matrix()
{
    return canvas;
}
*/
