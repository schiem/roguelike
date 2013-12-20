#include "dungeon.h"
#include <stdlib.h>
using namespace tiledef;

Dungeon::Dungeon()
{
    dungeon = TileMatrix(MAX_HEIGHT, std::vector<Tile>(MAX_WIDTH, BLOCK_WALL));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    width = 10;
    height= 10;
    down_stair = std::vector<int>(2);
    up_stair = std::vector<int>(2);
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            dungeon[i][j] = DIRT;
        }
    }
}

Dungeon::Dungeon(const Dungeon& d)
{
    dungeon = d.dungeon;
    width = d.width;
    height = d.height;
    down_stair = d.down_stair;
    up_stair = d.up_stair;
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = d.num_rooms;
    for(int i = 0; i < d.num_rooms; i++) {
        this->rooms[i] = d.rooms[i];
    }
}

Dungeon::Dungeon(int _width, int _height)
{
    dungeon = TileMatrix(MAX_HEIGHT, std::vector<Tile>(MAX_WIDTH, BLOCK_WALL));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    width = _width;
    height = _height;
    down_stair = std::vector<int>(2);
    up_stair = std::vector<int>(2);
    for(int i = 0; i < _height; i++) {
        for(int j = 0; j < _width; j++) {
            dungeon[i][j] = BLOCK_WALL;
            if((j == 0) || (j == _width - 1)) {
                dungeon[i][j] = DIRT;
            }
            if((i == 0) || (i == _height - 1)) {
                dungeon[i][j] = DIRT;
            }
        }
    }
}



Dungeon& Dungeon::operator= (const Dungeon& d){
    dungeon = d.dungeon;
    width = d.width;
    height = d.height;
    down_stair = d.down_stair;
    up_stair = d.up_stair;
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = d.num_rooms;
    for(int i = 0; i < 2; i++) {
        this->rooms[i] = d.rooms[i];
    }
    return *this;
}

/**
 * PRE: Will be given two integers representing a row and column.
 * POST: Will run the given point in the dungeon through a series of assertions
 * to ensure that it is valid.
 */
void Dungeon::tile_assertions(int row, int col) const {
    assert(row >= 0);
    assert(row < height);
    assert(col >= 0);
    assert(col < width);
}

/**
 * PRE: Given a bool whether there is a dungeon below it in the chunk.
 * POST: Will create a set of upstairs in a random rooms, and a set of
 * downstairs if there is a chunk below it.
 * 
 * Also contains a dungeon dump, to view the coordinates of the rooms in
 * the dungeon which this method is called on.  Currently commented out.
 */


void Dungeon::make_stairs(bool is_dungeon){
    assert(num_rooms > 0);
    
    //This is currently 2 because only 2 rooms are initialized.
    Room up_room = rooms[rand() % 2];
    Room down_room = rooms[rand() % 2];
    
    //Room up_room = rooms[rand() % num_rooms];
    //Room down_room = rooms[rand() % num_rooms];
    
    for (int i=0; i<num_rooms;i++)
    {
        cout<<i<<": "<<rooms[i].tl.col<<", "<<rooms[i].tl.row<<endl;
        cout<<i<<": "<<rooms[i].br.col<<", "<<rooms[i].br.row<<endl;
    }
    //Find the locations of up/down stairs. 
    up_stair[0] = up_room.tl.col + rand() % (up_room.br.col - up_room.tl.col);
    up_stair[1] = up_room.tl.row + rand() % (up_room.br.row - up_room.tl.row);
    if(is_dungeon)
    {
        down_stair[0] = down_room.tl.col + (rand() % (down_room.br.col - down_room.tl.col));
        down_stair[1] = down_room.tl.row + (rand() % (down_room.br.row - down_room.tl.row));
        dungeon[down_stair[1]][down_stair[0]] = DOWN_STAIR;
    }
    dungeon[up_stair[1]][up_stair[0]] = UP_STAIR;

}


/**
 * self-explanatory getters
 */
Tile Dungeon::get_tile(int row, int col) const {
    tile_assertions(row, col);
    return dungeon[row][col];
}

Tile Dungeon::get_tile(IntPoint point) const {
    tile_assertions(point.row, point.col);
    return dungeon[point.row][point.col];
}

/**
 * self-explanatory setters
 */
void Dungeon::set_tile(int row, int col, Tile theTile) {
    tile_assertions(row, col);
    dungeon[row][col] = theTile;
}

void Dungeon::set_tile(IntPoint point, Tile theTile) {
    tile_assertions(point.row, point.col);
    dungeon[point.row][point.col] = theTile;
}

const std::vector<std::vector<Tile> >&  Dungeon::get_dungeon(){
    return dungeon;
}

