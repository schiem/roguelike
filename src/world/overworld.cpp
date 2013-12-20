#include <overworld.h>
using namespace tiledef;
Overworld::Overworld() {
    width = 10;
    height = 10;
    is_dungeon = false;
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
        int door_x = rand() % width;
        int door_y = rand() % height;
        ground[door_y][door_x] = DOWN_STAIR;
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

