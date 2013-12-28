#include <overworld.h>
using namespace tiledef;
using namespace enemies;
Overworld::Overworld() {
    width = 10;
    height = 10;
    has_layer_below = false;
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
    down_stair.col = rand() % width;
    down_stair.row = rand() % height;
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
        ground[down_stair.row][down_stair.col] = DOWN_STAIR;
    }
    int spawn_x = rand() % width;
    int spawn_y = rand() % height;
    spawner = Spawner(spawn_x, spawn_y, -1, Kobold);
    ground[spawn_y][spawn_x] = KOBOLD_SPAWNER;
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


Spawner Overworld::get_spawner()
{
    return spawner;
}

