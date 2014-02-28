/**
 *  OVERWORLD.CPP
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    int attack;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    


    if (tile_type == map_tile::MAP_DEFAULT) {
        build_land_overworld();
    } else if (tile_type == map_tile::MAP_WATER) {
        build_water_overworld();
    } else if (tile_type == map_tile::MAP_BEACH) {
        build_beach_overworld();
    } else if (tile_type == map_tile::MAP_FOREST) {
        build_forest_overworld();
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
    down_stair.col = rand() % width;
    down_stair.row = rand() % height;
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

void Overworld::build_forest_overworld() {
    /*
     *TODO:
     * -Put in a smaller bresenham circle to fill the gaps for water in groves
     * -Probably make it so that stairs and spawners don't spawn where they shouldn't
     * -Play around with numbers for a prettier effect
     * -Bresenham ellipses?
     */
    
    /*TESTING ITEMS*/
    IntPoint coords;
    do{
        coords = IntPoint(rand() % height, rand() % width);
    }
    while (ground[coords.row][coords.col].can_be_moved_through == false);
    items.push_back(new Equipment(coords, equipment::boots));


    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            ground[i][j] = GRASS_DIRT;
        }
    }
    
    int num_trees = (rand() % 30) + 100;
    for(int i=0;i<num_trees;i++)
    {
        int x_coord = rand() % (width-1);
        int y_coord = rand() % (height-1);
        for(int row=y_coord;row<y_coord+2;row++)
        {
            for(int col=x_coord;col<x_coord+2;col++)
            {
                ground[row][col] = BIG_TREE;
            }
        }
    }

    if(rand() % 1 == 0)
    {
       //spawn a grove!
       int radius = (rand() % 5) + 10;
       int start_x = (rand() % (width - 2 * (radius+1)) + radius);
       int start_y = (rand() % (height - 2 * (radius+1)) + radius);
       IntPoint start = IntPoint(start_y, start_x);
       for(int i=start_y-radius;i<start_y+radius;i++)
       {
           for(int j=start_x-radius;j<start_x+radius;j++)
           {
               ground[i][j] = GRASS_DIRT;
           }
       }
       std::vector<IntPoint> out_circle = bresenham_circle(start, radius);
       for(int i=0;i<out_circle.size();i+=3)
       {
            for(int row=out_circle[i].row;row<out_circle[i].row+2;row++)
            {
                for(int col=out_circle[i].col;col<out_circle[i].col+2;col++)
                {
                    ground[row][col] = BIG_TREE;
                }
            }
        }
        int new_rad = radius - ((rand() % 3) + 5);
        std::vector<IntPoint> in_circle = bresenham_circle(start, new_rad);
        for(int i=0;i<in_circle.size();i++)
        {
            std::vector<IntPoint> line = bresenham_line(start, in_circle[i]);
            for(int j=0;j<line.size();j++)
            {
                ground[line[j].row][line[j].col] = WATER;
            }
        }
        for(int i=start_y-new_rad-1;i<start_y+new_rad+1;i++)
        {
            for(int j=start_x-new_rad-1;j<start_x+new_rad+1;j++)
            {
                int num = 0;
                num += (ground[i-1][j-1] == WATER) + (ground[i-1][j] == WATER) + (ground[i-1][j+1] == WATER) +
                    (ground[i][j-1] == WATER) + (ground[i][j+1] == WATER) + (ground[i+1][j-1] == WATER) + 
                    (ground[i+1][j] == WATER) + (ground[i+1][j+1] == WATER);
                //a threshold of 4 or above gives normal circular pools
                //a threshold of 2 gives slightly odd pools
                //a threshold of 1 gives very oddly shaped pools
                if(smoothing_pass(6, WATER, num))
                {
                    ground[i][j] = WATER;
                }
            }
        }
    }

    down_stair.col = rand() % width;
    down_stair.row = rand() % height;
    if(has_layer_below) {
        ground[down_stair.row][down_stair.col] = DOWN_STAIR;
    }
    int spawn_x = rand() % width;
    int spawn_y = rand() % height;
    spawner = Spawner(spawn_x, spawn_y, -1, Kobold);
    ground[spawn_y][spawn_x] = KOBOLD_SPAWNER;
}

bool Overworld::smoothing_pass(int threshold, Tile tile_type, int num)
{
    if(num >= threshold) {
        if (rand() % (8 - (num -1)) == 0) {
            return true;
        }
    }
    return false;
}

Tile* Overworld::get_tile(int row, int col) {
    return &ground[row][col];
}

vector<Item*>* Overworld::get_items()
{
    return &items;
}


void Overworld::set_tile(int row, int col, Tile tile) {
    ground[row][col] = tile;
}

std::vector<std::vector<Tile> >& Overworld::get_ground() {
    return ground;
}


Spawner Overworld::get_spawner()
{
    return spawner;
}

