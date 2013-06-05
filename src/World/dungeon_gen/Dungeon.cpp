#include "Dungeon.h"
using namespace tiledef;

Dungeon::Dungeon()
{
    dungeon = TileMatrix(MAX_HEIGHT, std::vector<Tile>(MAX_WIDTH, EMPTY));
    num_rooms = 0;
    width = 10;
    height= 10;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            dungeon[i][j] = DIRT;
        }
    }
}


Dungeon::Dungeon(int _width, int _height)
{
    dungeon = TileMatrix(MAX_HEIGHT, std::vector<Tile>(MAX_WIDTH, EMPTY));
    num_rooms = 0;
    width = _width;
    height = _height;
    for(int i = 0; i < _height; i++)
    {
        for(int j = 0; j < _width; j++)
        {
            dungeon[i][j] = EMPTY;
            if((j == 0) || (j == _width - 1)) {
                dungeon[i][j] = DIRT;
            }
            if((i == 0) || (i == _height - 1)) {
                dungeon[i][j] = DIRT;
            }
        }
    }
}

Tile Dungeon::get_tile(int row, int col) const
{
    return dungeon[row][col];
}

Tile Dungeon::get_tile(IntPoint point) const
{
    return dungeon[point.row][point.col];
}

void Dungeon::set_tile(int row, int col, Tile theTile)
{
    dungeon[row][col] = theTile;
}

void Dungeon::set_tile(IntPoint point, Tile theTile)
{
    dungeon[point.row][point.col] = theTile;
}
