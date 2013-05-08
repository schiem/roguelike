#include <Dungeon.h>

Dungeon::Dungeon()
{

}

Dungeon::Dungeon(int _width, int _height)
{
    width = _width;
    height = _height;
    for(int i = 0; i < _height; i++)
    {
        for(int j = 0; j < _width; j++)
        {
            dungeon[i][j] = EMPTY;
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
