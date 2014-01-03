/**
 *  DUNGEON.CPP
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

#include <dungeon.h>
#include <stdlib.h>
using namespace tiledef;
using namespace enemies;

Dungeon::Dungeon()
{
    dungeon = TileMatrix(30, std::vector<Tile>(30, BLOCK_WALL));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    width = 10;
    height= 10;
    down_stair = IntPoint(0, 0);
    up_stair = IntPoint(0, 0);
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            dungeon[i][j] = DIRT;
        }
    }
}

Dungeon::Dungeon(int _width, int _height)
{
    width = _width;
    height = _height;
    dungeon = TileMatrix(height, std::vector<Tile>(width, BLOCK_WALL));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    down_stair = IntPoint(0, 0);
    up_stair = IntPoint(0, 0);
    for(int i = 0; i < _height; i++) {
        for(int j = 0; j < _width; j++) {
            dungeon[i][j] = BLOCK_WALL;
        }
    }
}


Dungeon::Dungeon(const Dungeon& d)
{
    dungeon = d.dungeon;
    width = d.width;
    height = d.height;
    down_stair = d.down_stair;
    up_stair = d.up_stair;
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = d.num_rooms;
    spawner = d.spawner;
    for(int i = 0; i < d.num_rooms; i++) {
        this->rooms[i] = d.rooms[i];
    }
}



Dungeon& Dungeon::operator= (const Dungeon& d){
    dungeon = d.dungeon;
    width = d.width;
    height = d.height;
    down_stair = d.down_stair;
    up_stair = d.up_stair;
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = d.num_rooms;
    spawner = d.spawner;
    for(int i = 0; i < num_rooms; i++) {
        this->rooms[i] = d.rooms[i];
    }
    return *this;
}

/**
 * PRE: Will be given two integers representing a row and column.
 * POST: Will run the given point in the dungeon through a series of assertions
 * to ensure that it is valid.
 */
void Dungeon::tile_assertions(int row, int col) const {
    assert(row >= 0);
    assert(row < height);
    assert(col >= 0);
    assert(col < width);
}

/**
 * PRE: Given a bool whether there is a dungeon below it in the chunk.
 * POST: Will create a set of upstairs in a random rooms, and a set of
 * downstairs if there is a chunk below it.
 *
 * Also contains a dungeon dump, to view the coordinates of the rooms in
 * the dungeon which this method is called on.  Currently commented out.
 */
void Dungeon::make_stairs(bool has_layer_below){
    assert(num_rooms > 0);

    Room up_room = rooms[rand() % num_rooms];
    Room down_room = rooms[rand() % num_rooms];

    //Find the locations of up/down stairs.
    up_stair.col = 1 + up_room.tl.col + rand() % ((up_room.br.col - 1) - (up_room.tl.col + 1));
    up_stair.row = 1 + up_room.tl.row + rand() % ((up_room.br.row - 1) - (up_room.tl.row + 1));

    if(has_layer_below) {
        do{
         down_stair.col = 1 + down_room.tl.col +
            (rand() % ((down_room.br.col - 1) - (down_room.tl.col + 1)));
        down_stair.row = 1 + down_room.tl.row +
            (rand() % ((down_room.br.row - 1) - (down_room.tl.row + 1)));
        }
        while(down_stair == up_stair);
        dungeon[down_stair.row][down_stair.col] = DOWN_STAIR;
    }
    dungeon[up_stair.row][up_stair.col] = UP_STAIR;
}


void Dungeon::make_spawner(int _depth)
{
    Room spawn_room;
    do
    {
        spawn_room = rooms[rand() % num_rooms];
    }
    while(spawn_room.br.row-spawn_room.tl.row<4 && spawn_room.br.col-spawn_room.tl.col<4);
    IntPoint spawn;
    do{
    spawn = IntPoint(2 + spawn_room.tl.row + rand() % ((spawn_room.br.row - 2) - (spawn_room.tl.row + 2)), 2 + spawn_room.tl.col + rand() % ((spawn_room.br.col - 2) - (spawn_room.tl.col + 2)));
    }
    while(spawn==down_stair || spawn == up_stair);
    spawner = Spawner(spawn.col, spawn.row, _depth, Kobold);
    dungeon[spawn.row][spawn.col] = KOBOLD_SPAWNER;
}

/**
 * PRE: None
 * POST: Will replace the outer edge of the dungeon with a border.
 */
void Dungeon::make_border() {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if((j == 0) || (j == width - 1)) {
                dungeon[i][j] = DUNGEON_BORDER;
            }
            if((i == 0) || (i == height - 1)) {
                dungeon[i][j] = DUNGEON_BORDER;
            }
        }
    }
}


/**
 * self-explanatory getters
 */
Tile Dungeon::get_tile(int row, int col) const {
    tile_assertions(row, col);
    return dungeon[row][col];
}

Tile Dungeon::get_tile(IntPoint point) const {
    tile_assertions(point.row, point.col);
    return dungeon[point.row][point.col];
}

Tile* Dungeon::get_tile_pointer(int row, int col) {
    tile_assertions(row, col);
    return &dungeon[row][col];
}

/**
 * self-explanatory setters
 */
void Dungeon::set_tile(int row, int col, Tile theTile) {
    tile_assertions(row, col);
    dungeon[row][col] = theTile;
}

void Dungeon::set_tile(IntPoint point, Tile theTile) {
    tile_assertions(point.row, point.col);
    dungeon[point.row][point.col] = theTile;
}

const std::vector<std::vector<Tile> >&  Dungeon::get_dungeon(){
    return dungeon;
}

Spawner Dungeon::get_spawner()
{
    return spawner;
}


void Dungeon::dungeon_dump()
{
    //dungeon dump
    int tile;
    for(int row=0;row<dungeon.size();row++)
    {
        for(int col=0;col<dungeon[row].size();col++)
        {
            tile = dungeon[row][col].tile_id;
            switch(tile){
                case 2:
                    cout<<".";
                    break;
                case 4:
                    cout<<"#";
                    break;
                case 12:
                    cout<<"#";
                    break;
                case 13:
                    cout<<"d";
                    break;
                case 30:
                    cout<<"u";
                    break;
                case 16:
                    cout<<"S";
                    break;
                default:
                    cout<<" ";
                    break;
            }
        }
        cout<<endl;
    }
    cout<<"---------------------------------------------"<<endl;
}
