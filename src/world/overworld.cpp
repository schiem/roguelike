#include <overworld.h>
using namespace tiledef;
Overworld::Overworld() {
    width = 10;
    height = 10;
    is_dungeon = false;
    down_stair = std::vector<int>(2);
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 8 == 0) {
                ground[i][j] = TREE;
            } else {
                ground[i][j] = OVERWORLD_DIRT;
            }
        }
    }
}

Overworld::Overworld(int _width, int _height, bool _is_dungeon) {
    width = _width;
    height = _height;
    is_dungeon = _is_dungeon;
    down_stair = std::vector<int>(2);
    down_stair[0] = rand() % width;
    down_stair[1] = rand() % height;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 8 == 0){
                ground[i][j] = TREE;
            } else {
                ground[i][j] = OVERWORLD_DIRT;
            }
        }
    }
    if(is_dungeon)
    {
        ground[down_stair[1]][down_stair[0]] = DOWN_STAIR;
    }
}

Tile Overworld::get_tile(int row, int col) const {
    return ground[row][col];
}

void Overworld::set_tile(int row, int col, Tile tile) {
    ground[row][col] = tile;
}

const std::vector<std::vector<Tile> >& Overworld::get_ground() {
    return ground;
}

