#include "Canvas.h"

using namespace std;
using namespace tiledef;

Canvas::Canvas()
{
    db = DungeonBuilder(80, 40);
    db.build_pblind_dungeon(5, 5, 5);
    dungeon = db.get_dungeon();

    main_char = Main_Character(100, 1, 1, 3);

    canvas = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH));
}

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
const std::vector<std::vector<Tile> >& Canvas::get_matrix()
{
    return canvas;
}
