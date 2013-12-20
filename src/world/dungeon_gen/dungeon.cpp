#include "dungeon.h"
using namespace tiledef;

Dungeon::Dungeon()
{
    dungeon = TileMatrix(MAX_HEIGHT, std::vector<Tile>(MAX_WIDTH, BLOCK_WALL));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    width = 10;
    height= 10;
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

