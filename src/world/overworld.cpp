#include <overworld.h>
using namespace tiledef;
Overworld::Overworld() {
    width = 10;
    height = 10;
    has_layer_below = false;
    down_stair = std::vector<int>(2);
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 10 == 0) {
                ground[i][j] = TREE;
            } else {
                ground[i][j] = OVERWORLD_DIRT;
            }
        }
    }
}

Overworld::Overworld(int _width, int _height, bool _has_layer_below) {
    width = _width;
    height = _height;
    has_layer_below = _has_layer_below;
    down_stair = std::vector<int>(2);
    down_stair[0] = rand() % width;
    down_stair[1] = rand() % height;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 20 == 0){
                ground[i][j] = TREE;
            } else {
                ground[i][j] = OVERWORLD_DIRT;
            }
        }
    }
    if(has_layer_below) {
        ground[down_stair[1]][down_stair[0]] = DOWN_STAIR;
    }
}

Tile* Overworld::get_tile(int row, int col) {
    return &ground[row][col];
}

void Overworld::set_tile(int row, int col, Tile* tile) {
    ground[row][col] = *tile;
}

std::vector<std::vector<Tile> >& Overworld::get_ground() {
    return ground;
}

