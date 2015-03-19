/**
 *  @file WORLD_MAP.CPP
 *  @author Seth A. Yoder
 *
 *  @section LICENSE
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

#include <world_map.h>

using namespace std;

WorldMap::WorldMap() {
    srand(time(NULL));
    height = WORLD_HEIGHT;
    width = WORLD_WIDTH;
    map = std::vector<std::vector<MapTile> >(height,
            std::vector<MapTile>(width));

    generate_land_mass();
    generate_beaches();
}

const std::vector<std::vector<MapTile> >& WorldMap::get_map() {
    return map;
}

bool WorldMap::out_of_bounds(int row, int col) {
    return (row < 0 ||
            row >= height ||
            col < 0 ||
            col >= width);
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

int WorldMap::count_not_surrounding_tiles(int row, int col, MapTile tile_type)
{
    return (8 - count_in_surrounding_tiles(row, col, tile_type));
}

void WorldMap::starting_noise(int border) {
    for(int i = border; i < (height - border); i++) {
        for(int j = border; j < (width - border); j++) {
            map[i][j] = random_weighted_tile(); 
        }
    }
    
    /**
    for(int i = 1; i < (height - 1); i++) {
        for(int j = 1; j < (width - 1); j++) {
            if(map[i+1][j] == map[i][j+1] &&
                map[i-1][j] == map[i][j-1] &&
                map[i][j+1] == map[i-1][j]) {

                map[i][j] = map[i][j+1];
            }
        }
    }
    */
}

void WorldMap::smoothing_pass(MapTile tile_type, int threshold) {
    int num;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            num = count_in_surrounding_tiles(i, j, tile_type);
            if(num >= threshold) {
                //what is this doing?!
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
    cout<<height<<" "<<width<<endl;
    for(int i=0;i<map_tile::NUM_MAP_TILE - 1;i++)
    {
        MapTile tile = map_tile::MAP_TILE_INDEX[i]; 
        if(tile.smoothed)
        {
            for(int j = 0;j < 10; j++) {
                smoothing_pass(map_tile::MAP_TILE_INDEX[i], tile.smooth_amount);
            }
        }
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_WATER, 3);
    }
    //Tweaking any values here can give vastly different results.
    /**
    for(int i = 0; i < 15; i++) {
        smoothing_pass(map_tile::MAP_WATER, 4);
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_FOREST, 3);
    }
    for(int i = 0; i < 17; i++) {
        smoothing_pass(map_tile::MAP_WATER, 2);
    }
    smoothing_pass(map_tile::MAP_FOREST, 2);
    for(int i = 0; i < 12; i++) {
        smoothing_pass(map_tile::MAP_FOREST, 3);
    }
    **/
}


void WorldMap::ocean_borders(int border) {
    for(int i = 0; i < border; i++) {
        for(int j = 0; j < width; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }
    }

    for(int i = (height - border); i < height; i++) {
        for(int j = 0; j < width; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }
    }

    for(int i = border; i < (height - border); i++) {
        for(int j = 0; j < border; j++) {
            map[i][j] = map_tile::MAP_WATER;
        }

        for(int j = (width - border); j < width; j++){
            map[i][j] = map_tile::MAP_WATER;
        }
    }

}

void WorldMap::generate_beaches() {
    for(int i=1;i<height - 2;i++) {
        for(int j=1;j<width - 2;j++) {
            std::cout<<count_not_surrounding_tiles(i, j, map_tile::MAP_WATER)<<std::endl;
            //get the number of beaches
            int num_beaches = count_in_surrounding_tiles(i, j, map_tile::MAP_BEACH);
            //get the non water tiles
            int num_land = count_not_surrounding_tiles(i, j, map_tile::MAP_WATER);
            
            //we don't want beaches to count towards making another beach, there's already
            //one there.
            int countable_land = num_land - num_beaches;
           
            if((map[i][j] == map_tile::MAP_WATER) && countable_land > 0 && countable_land < 6) {
                map[i][j] = map_tile::MAP_BEACH;
            }
        }
    }
}

MapTile WorldMap::tile_at(int row, int col)
{
    return map[row][col];
}

std::vector<std::vector<IntPoint> > WorldMap::find_contiguous(MapTile target)
{
    std::vector<IntPoint> closed_list;
    std::vector<std::vector<IntPoint> > contiguous;
    for(int i=0;i<map.size();i++)
    {
        for(int j=0;j<map[i].size();j++)
        {
            if(map[i][j] == target && !is_in(closed_list, IntPoint(i, j)))
            {
                std::vector<IntPoint> cur_contig;
                flood(IntPoint(i, j), closed_list, cur_contig, target);
                contiguous.push_back(cur_contig);
            }
        }
    }
    return contiguous;
}

void WorldMap::flood(IntPoint start_point, std::vector<IntPoint>& closed, std::vector<IntPoint>& cur_contig, MapTile target)
{
    cur_contig.push_back(start_point);
    closed.push_back(start_point);
    for(int i=(start_point.row - 1);i<(start_point.row + 2);i++)
    {
        for(int j=(start_point.col - 1);j<(start_point.col + 2); j++)
        {
            if(map[i][j] == target && !is_in(closed, IntPoint(i, j)))
            {
                flood(IntPoint(i, j), closed, cur_contig, target);
            }
        }
    }
}

MapTile WorldMap::random_weighted_tile()
{
    //it seems ridiculous that I'm constructing this
    //so many times when I'll be relying on it...should
    //I just pass these in?
    std::vector<MapTile> tile_list;
    int total_tiles = 0;
    for(int i=0;i<map_tile::NUM_MAP_TILE;i++)
    {
        for(int j=0;j<map_tile::MAP_TILE_INDEX[i].weight;i++)
        {
            tile_list.push_back(map_tile::MAP_TILE_INDEX[i]);
            total_tiles += 1;
        }
    }
    return tile_list[rand() % total_tiles];
}

