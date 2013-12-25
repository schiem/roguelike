#include "world_map.h"

namespace map_tile {
    MapTile MAP_DEFAULT = {146, GREEN};
    MapTile MAP_WATER = {247, BLUE};
}

using namespace std;

WorldMap::WorldMap() {

}

WorldMap::WorldMap(int h, int w) {
    srand(time(NULL));
    height = h;
    width = w;
    map = std::vector<std::vector<MapTile> >(height, 
            std::vector<MapTile>(width));

    generate_land_mass();

}

std::vector<std::vector<MapTile> > WorldMap::get_map() const {
    return map;
}

bool WorldMap::out_of_bounds(int row, int col) {
    return (row < 0 ||
            row >= height ||
            col < 0 ||
            col >= height);
}

void WorldMap::set_land_or_water(int row, int col, 
                                 int mod, bool more_water) {
    if(more_water) {
        if(rand() % mod == 0) {
            map[row][col] = map_tile::MAP_DEFAULT;
        } else {
            map[row][col] = map_tile::MAP_WATER;
        }
    } else {
        if(rand() % mod == 0) {
            map[row][col] = map_tile::MAP_WATER;
        } else {
            map[row][col] = map_tile::MAP_DEFAULT;
        }
    }
}

void WorldMap::ocean_borders(int border) {
    for(int i = 0; i < border; i++) {
        for(int j = 0; j < width; j++) {
            set_land_or_water(i, j, 15, true);
        }
    }

    for(int i = (height - border); i < height; i++) {
        for(int j = 0; j < width; j++) {
            set_land_or_water(i, j, 15, true);
        }
    }

    for(int i = border; i < (height - border); i++) {
        for(int j = 0; j < border; j++) {
            set_land_or_water(i, j, 15, true);
        }

        for(int j = (width - border); j < width; j++){
            set_land_or_water(i, j, 15, true);
        }
    }

}

int WorldMap::count_in_surrounding_tiles(int row, int col, 
                                        MapTile tile_type) {
    int num = 0;
    int newrow, newcol;
    for(int i = -1; i < 2; i++) {
        for(int j = -1; j < 2; j++) {
            if(!(i==0 && j==0)) {
                newrow = row + i;
                newcol = col + j;
                if(!out_of_bounds(newrow, newcol)) {
                    if(map[newrow][newcol] == tile_type) {
                        num++;
                    }
                }
            }
        }
    }

    return num;
}

void WorldMap::starting_noise(int border) {
    for(int i = border; i < (height - border); i++) {
        for(int j = border; j < (width - border); j++) {
            set_land_or_water(i, j, 3, false);
        }
    }

    for(int i = 1; i < (height - 1); i++) {
        for(int j = 1; j < (width - 1); j++) {
            if(map[i+1][j] == map[i][j+1] &&
                map[i-1][j] == map[i][j-1] &&
                map[i][j+1] == map[i-1][j]) {

                map[i][j] = map[i][j+1];
            }
        }
    }
}

void WorldMap::smoothing_pass(MapTile tile_type) {
    int num;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            num = count_in_surrounding_tiles(i, j, tile_type);
            if(num >= 4) {
                if(rand() % (8 - (num - 1)) == 0) {
                    map[i][j] = tile_type;
                }
            }
        }
    }
}

void WorldMap::generate_land_mass() {
    int border_size = 5;
    ocean_borders(border_size);
    starting_noise(border_size);
    smoothing_pass(map_tile::MAP_WATER);
    smoothing_pass(map_tile::MAP_DEFAULT);
}
