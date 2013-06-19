#include "corruptible_pblind_db.h"

using namespace std;
using namespace tiledef;
//Default constructor
CorruptiblePBlindDB::CorruptiblePBlindDB()
{

}

/*
 * Just call the super constructor
 */
CorruptiblePBlindDB::CorruptiblePBlindDB(int _width, int _height, int seed) : 
    ProcedurallyBlindDB(_width, _height, seed)
{

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
        
        //push some corners in.
        //std::vector.size() returns an unsigned long...
        for(unsigned long j = 0; j < corners.size(); j++) {
            if (rolled_over(10)) {
                main_dungeon.set_tile(corners[j], DIRT);
            } else if (rolled_over(10)) {
                main_dungeon.set_tile(corners[j], DIRT);
                //There are a number of ways I could do this. I'm going with the weirdest, because YOLO.
                //This is for the first rule listed up above.
                int vert = 0; int horiz = 0;
                //If the block below this point is a WALL block...
                if(main_dungeon.get_tile(corners[j].row + 1, corners[j].col) == WALL) {
                    vert = 1;
                } else {
                    //the block above this point is a WALL block... I hope. TODO watch out for room clusters!!
                }
            }
        }
    }   
}

void CorruptiblePBlindDB::build_dungeon(int target, int deviation, int squareness)
{   
    reset();
    bool dungeon_is_awesome;
    build_start_room();
    int tries = 0;
    do {
        tries++;
        dungeon_is_awesome = true;
        build_dungeon_recursive(target, deviation, squareness);
        if (num_rooms < (target - 3)) {
            dungeon_is_awesome = false;
            main_dungeon = Dungeon(width, height);
            num_rooms = 0;
        }
    } while(!dungeon_is_awesome);
}
