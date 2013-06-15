#include "corruptible_pblind_db.h"

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
