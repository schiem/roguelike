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
CorruptiblePBlindDB::CorruptiblePBlindDB(int _width, int _height, int _target_rooms, int seed) : 
    ProcedurallyBlindDB(_width, _height, _target_rooms, seed)
{

}

void CorruptiblePBlindDB::corrupt_corners(vector<IntPoint> corners)
{
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

            //If the block below this point is a WALL block...
            if((corners[j].row + 1 < main_dungeon.height) && (main_dungeon.get_tile(corners[j].row + 1, corners[j].col) == WALL)) {
                vert = 1;
                wall_blocks_found++;
            } 
            if ((corners[j].row - 1 >= 0) && (main_dungeon.get_tile(corners[j].row - 1, corners[j].col) == WALL)) {
                vert = -1;
                wall_blocks_found++;
            } 
            if ((corners[j].col + 1 < main_dungeon.width) && (main_dungeon.get_tile(corners[j].row, corners[j].col + 1) == WALL)) {
                horiz = 1;
                wall_blocks_found++;
            } 
            if ((corners[j].col + 1 < main_dungeon.width) && (main_dungeon.get_tile(corners[j].row, corners[j].col - 1) == WALL)) {
                horiz = -1;
                wall_blocks_found++;
            }

            if (wall_blocks_found != 2) {
                return;
            } else {
                main_dungeon.set_tile(corners[j], EMPTY);
                main_dungeon.set_tile(corners[j].row + vert, corners[j].col + horiz, WALL);
            }
        }
    }
}

void CorruptiblePBlindDB::corrupt_walls(Room& room) {
    int static_col;
    int mod;
    for(int i = 0; i < 2; i++) {
        if(i == 0) {
            //Loop through the blocks on the left wall
            static_col = room.tl.col;
            mod = 1;
        } else if (i == 1) {
            //Loop through the blocks on the right wall
            static_col = room.br.col;
            mod = -1;
        }
        for(int cur_row = room.tl.row + 1; cur_row < room.br.row; cur_row++) {
            if( (abs(cur_row - room.tl.row) > 1) && (abs(cur_row - room.br.row) > 1) &&
                (main_dungeon.get_tile(cur_row - 1, static_col) == WALL) && 
                (main_dungeon.get_tile(cur_row + 1, static_col) == WALL) &&
                (main_dungeon.get_tile(cur_row - 1, static_col + mod) == DIRT) &&
                (main_dungeon.get_tile(cur_row + 1, static_col + mod) == DIRT) &&
                (main_dungeon.get_tile(cur_row, static_col + mod) == DIRT)) {
                if(rolled_over(5)) {
                    //rule 1
                    main_dungeon.set_tile(cur_row, static_col, EMPTY);
                    main_dungeon.set_tile(cur_row, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row + 1, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row - 1, static_col + mod, WALL);
                } else if (rolled_over(5)) {
                    //rule 2
                    main_dungeon.set_tile(cur_row, static_col, EMPTY);
                    main_dungeon.set_tile(cur_row, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row - 1, static_col + mod, DIRT);
                    main_dungeon.set_tile(cur_row + 1, static_col + mod, DIRT);
                } else if (rolled_over(5)) {
                    //rule 3
                    main_dungeon.set_tile(cur_row, static_col, EMPTY);
                    main_dungeon.set_tile(cur_row, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row + 1, static_col + mod, DIRT);
                    main_dungeon.set_tile(cur_row - 1, static_col + mod, WALL);
                } else if (rolled_over(5)) {
                    main_dungeon.set_tile(cur_row, static_col, EMPTY);
                    main_dungeon.set_tile(cur_row, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row + 1, static_col + mod, WALL);
                    main_dungeon.set_tile(cur_row - 1, static_col + mod, DIRT);
                }
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
void CorruptiblePBlindDB::corrupt_dungeon()
{ 
    //Loop through the rooms.
    for(int i = 0; i < num_rooms; i++) {
        Room& current_room = main_dungeon.rooms[i];
        corrupt_walls(current_room);

        IntPoint tl = current_room.tl;
        IntPoint tr = IntPoint(current_room.tl.row, current_room.br.col);
        IntPoint bl = IntPoint(current_room.br.row, current_room.tl.col);
        IntPoint br = current_room.br;
        vector<IntPoint> corners = vector<IntPoint>(4);
        corners[0] = tl; corners[1] = tr; corners[2] = bl; corners[3] = br;
        corrupt_corners(corners);
    }   
}

void CorruptiblePBlindDB::build_dungeon()
{   
    reset();
    build_start_room();
    bool dungeon_is_awesome;
    int tries = 0;
    do {
        tries++;
        dungeon_is_awesome = true;
        build_dungeon_recursive(target_rooms);
        corrupt_dungeon();
        if (num_rooms < (target_rooms - 3)) {
            dungeon_is_awesome = false;
            main_dungeon = Dungeon(width, height, target_rooms);
            num_rooms = 0;
        }
    } while(!dungeon_is_awesome);
}
