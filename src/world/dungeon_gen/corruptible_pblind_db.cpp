#include "corruptible_pblind_db.h"

using namespace std;
using namespace tiledef;
//Default constructor
/*
CorruptiblePBlindDB::CorruptiblePBlindDB() {

}
*/

/*
 * Just call the super constructor
 */
CorruptiblePBlindDB::CorruptiblePBlindDB(int _width, int _height, int seed) : 
    ProcedurallyBlindDB(_width, _height, seed) {

}

void CorruptiblePBlindDB::corrupt_corners(vector<IntPoint> corners) {
    int vert = 0;
    int horiz = 0;
    int wall_blocks_found = 0;
    //push some corners in.
    //std::vector.size() returns an unsigned long...
    for(size_t j = 0; j < corners.size(); j++) {
        if (rolled_over(20)) {
            main_dungeon.set_tile(corners[j], EMPTY);
            return;
        }

        if (rolled_over(40)) {
            vert = 0; horiz = 0; wall_blocks_found = 0;
            //There are a number of ways I could go about doing this. 
            //I'm going with the weirdest, because YOLO.
            //This is for the first rule listed above corrupt_walls().

            //If the block below this point is a ROOM_WALL block...
            if((corners[j].row + 1 < main_dungeon.height) && (main_dungeon.get_tile(corners[j].row + 1, corners[j].col) == ROOM_WALL)) {
                vert = 1;
                wall_blocks_found++;
            } 
            if ((corners[j].row - 1 >= 0) && (main_dungeon.get_tile(corners[j].row - 1, corners[j].col) == ROOM_WALL)) {
                vert = -1;
                wall_blocks_found++;
            } 
            if ((corners[j].col + 1 < main_dungeon.width) && (main_dungeon.get_tile(corners[j].row, corners[j].col + 1) == ROOM_WALL)) {
                horiz = 1;
                wall_blocks_found++;
            } 
            if ((corners[j].col + 1 < main_dungeon.width) && (main_dungeon.get_tile(corners[j].row, corners[j].col - 1) == ROOM_WALL)) {
                horiz = -1;
                wall_blocks_found++;
            }

            if (wall_blocks_found != 2) {
                return;
            } else {
                main_dungeon.set_tile(corners[j], EMPTY);
                main_dungeon.set_tile(corners[j].row + vert, corners[j].col + horiz, ROOM_WALL);
            }
        }
    }
}

/*
 * PRE: Will be called after a dungeon is built.
 * POST: Will "corrupt" dungeon walls according to the following rules:
 *
 * O = empty
 * X = wall
 * . = path
 * ? = anything
 *
 *   O O O   15%  O O O
 *   O X X  --->  O O X
 *   O X .        O X X
 *     |
 *     |     15%  O O O
 *     \------->  O O X
 *                O X .
 *
 *   O O O   5%   O O O
 *   X X X  --->  X O X
 *   ? . ?        X X X
 *     |
 *     |     5%
 *     |------->  O O O
 *     |          X O X
 *     |          . X .
 *     |
 *     |     5%
 *     |------->  O O O
 *     |          X O X
 *     |          . X X
 *     |
 *     |     5%
 *     \------->  O O O
 *                X O X
 *                X X .
 */
void CorruptiblePBlindDB::corrupt_walls()
{ 
    //Loop through the rooms.
    for(int i = 0; i < num_rooms; i++) {
        Room& current_room = main_dungeon.rooms[i];
        IntPoint tl = current_room.tl;
        IntPoint tr = IntPoint(current_room.tl.row, current_room.br.col);
        IntPoint bl = IntPoint(current_room.br.row, current_room.tl.col);
        IntPoint br = current_room.br;
        vector<IntPoint> corners = vector<IntPoint>(4);
        corners[0] = tl; corners[1] = tr; corners[2] = bl; corners[3] = br;
        corrupt_corners(corners);
    }   
}

void CorruptiblePBlindDB::build_dungeon(int target, int deviation)
{   
    reset();
    bool dungeon_is_awesome;
    build_start_room();
    int tries = 0;
    do {
        tries++;
        dungeon_is_awesome = true;
        build_dungeon_recursive(target, deviation);
        corrupt_walls();
        if (num_rooms < (target - 3)) {
            dungeon_is_awesome = false;
            main_dungeon = Dungeon(width, height);
            num_rooms = 0;
        }
    } while(!dungeon_is_awesome);
}
