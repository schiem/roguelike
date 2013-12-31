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

Overworld::Overworld(int _width, int _height, bool _has_layer_below, MapTile tile_type) {
    width = _width;
    height = _height;
    has_layer_below = _has_layer_below;

    down_stair.col = rand() % width;
    down_stair.row = rand() % height;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));

    if (tile_type == map_tile::MAP_DEFAULT) {
        build_land_overworld();
    } else if (tile_type == map_tile::MAP_WATER) {
        build_water_overworld();
    } else if (tile_type == map_tile::MAP_BEACH)
    {
        build_beach_overworld();
    }
}

void Overworld::build_land_overworld() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 8 == 0){
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

void Overworld::build_water_overworld() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (rand() % 4 == 0) {
                ground[i][j] = LIGHT_WATER;
            } else {
                ground[i][j] = WATER;
            }
        }
    }
}

void Overworld::build_beach_overworld() {
    for(int i = 0; i<height; i++) {
        for(int j = 0; j<width; j++)
        {
            if( rand() % 2 == 0) {
                ground[i][j] = SAND1;
            }
            else{
                ground[i][j] = SAND2;
            }
        }
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


Spawner Overworld::get_spawner()
{
    return spawner;
}

