#include <overworld.h>
using namespace tiledef;
Overworld::Overworld()
{
    width = 10;
    height = 10;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 8 == 0) {
                ground[i][j] = TREE;
            } else {
                ground[i][j] = DIRT;
            }
        }
    }
}

Overworld::Overworld(int _width, int _height)
{
    width = _width;
    height = _height;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 8 == 0){
                ground[i][j] = TREE;
            } else {
                ground[i][j] = DIRT;
            }
        }
        ground[1][1] = TREE;
    }
}

Tile Overworld::get_tile(int row, int col) const
{
    return ground[row][col];
}

void Overworld::set_tile(int row, int col, Tile tile)
{
    ground[row][col] = tile;
}

const std::vector<std::vector<Tile> >& Overworld::get_ground()
{
    return ground;
}

