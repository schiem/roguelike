/**
 *  @file CHUNK.CPP
 *  @author Michael & Seth Yoder
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

#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    height = CHUNK_HEIGHT;
    width = CHUNK_WIDTH;
}

Chunk::Chunk(MapTile tile_type, int _world_row, int _world_col) {
    height = CHUNK_HEIGHT;
    width = CHUNK_WIDTH;
    init(tile_type, _world_row, _world_col);
}

bool Chunk::find_serialized_chunk(int world_row, int world_col) {
    fs::path chunk_dir(CHUNK_DIR);

    stringstream ss;
    ss << CHUNK_DIR << "/chunk" << world_row << "_" <<world_col;
    string filename = ss.str();

    if(fs::exists(chunk_dir) && fs::is_directory(chunk_dir)) {
        if(fs::exists(filename)) {
            deserialize(filename, world_row, world_col);
            return true;
        }
    } else {
        cout<<"Chunk directory is missing. Aborting."<<endl;
        exit(EXIT_FAILURE);
    }

    return false;
}

void Chunk::deserialize(string file_name, int world_row, int world_col) {
    //Open the data file.
    ifstream chunk_data_file(file_name.c_str(),
            std::ifstream::in | std::ifstream::binary);

    //Stat the file to get the file size easily.
    int file_size = fs::file_size(file_name);
    //Initialize an array for the file data.
    /** \todo does this have to be dynamically allocated? */
    char * file_data = new char[file_size];
    //Read the entire file into the file_data array.
    chunk_data_file.read(file_data, file_size);

    int num_header_bytes=5; //Change this to stay relevant - must match 
                            //serialization functionality.

    /*
     * Simply reversing the deserialization.
     */
    width = file_data[0];
    height = file_data[1];
    chunk_depth = file_data[2];
    /*
     * The map tile ID was stored, now we just reverse it to get the chunk type.
     */
    chunk_type = map_tile::MAP_TILE_INDEX[file_data[3]];
    //Build the overworld.
    overworld = Overworld(width,height,(chunk_depth > 0),chunk_type);
    overworld.has_layer_below = file_data[4];


    dungeon_floors = vector<Dungeon>(chunk_depth, Dungeon(width, height));
    int current_byte = num_header_bytes;
    
    //There is a possibility of 2 bytes being stored for the overworld down
    //stair location.
    if(overworld.has_layer_below) {
        overworld.down_stair.row = file_data[current_byte];
        overworld.down_stair.col = file_data[current_byte + 1];
        current_byte += 2;
    }

    /*
     * Finding the dungeon stairs, if they have been serialized.
     */
    for(int i = 0; i < chunk_depth; i++) {
        dungeon_floors[i].spawner_loc.row = file_data[current_byte];
        dungeon_floors[i].spawner_loc.col = file_data[current_byte + 1];
        dungeon_floors[i].down_stair.row = file_data[current_byte + 2];
        dungeon_floors[i].down_stair.col = file_data[current_byte + 3];
        dungeon_floors[i].up_stair.row = file_data[current_byte + 4];
        dungeon_floors[i].up_stair.col = file_data[current_byte + 5];
        current_byte += 6;
    }

    cout<<chunk_depth<<"  "<<current_byte<<endl;
    assert(chunk_depth == ((current_byte - 4) / 6));

    Tile current_tile;
    unsigned int tile_id;
    bool seen;
    //Undo the serialization.
    for(int i = -1; i < chunk_depth; i++) {
        for(int j = 0; j < height; j++) {
            for(int k = 0; k < width; k++) {
                //The smallest bit will be 1 if seen = true.
                seen = (file_data[current_byte] & 1);
                //Then, we shift right by 1 bit to obtain the tile ID.
                tile_id = (file_data[current_byte] >> 1);
                current_tile=tiledef::TILE_INDEX[tile_id];
                current_tile.seen = seen;

                set_tile(i, j, k, current_tile);
                current_byte++;
            }
        }
    }
    //Cleaning up.
    delete [] file_data;
    chunk_data_file.close();
}

void Chunk::init(MapTile tile_type, int _world_row, int _world_col) {
    chunk_type = tile_type;
    world_row = _world_row;
    world_col = _world_col;

    bool found_chunk = find_serialized_chunk(world_row, world_col);
    if(!found_chunk) {
        if(tile_type == map_tile::MAP_DEFAULT) {
            build_land_chunk();
        } else if (tile_type == map_tile::MAP_WATER) {
            build_water_chunk();
        } else if (tile_type == map_tile::MAP_BEACH) {
            build_beach_chunk();
        } else if (tile_type == map_tile::MAP_FOREST) {
            build_forest_chunk();
        }
    }
}

void Chunk::build_land_chunk() {
    chunk_depth = rand() % 6 + 1;
    dungeon_floors = vector<Dungeon>(chunk_depth, Dungeon(width, height));
    Dungeon* temp_d;
    ProcedurallyBlindDB db(width, height);
    //CorruptiblePBlindDB db(width, height);

    bool has_layer_below;

    for (int i=0; i < chunk_depth; i++) {
        has_layer_below = (i < chunk_depth - 1);
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();

        dungeon_floors[i] = *temp_d;

        //makes the staircases in a dungeon
        dungeon_floors[i].make_stairs(has_layer_below);
        dungeon_floors[i].make_spawner(i);
    }
    //generate the overworld
    has_layer_below = (chunk_depth > 0);
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_DEFAULT);
}

void Chunk::build_forest_chunk() {
    chunk_depth = rand() % 6 + 1;
    dungeon_floors = vector<Dungeon>(chunk_depth, Dungeon(width, height));
    Dungeon* temp_d;
    ProcedurallyBlindDB db(width, height);
    //CorruptiblePBlindDB db(width, height);

    bool has_layer_below;

    for (int i=0; i < chunk_depth; i++) {
        has_layer_below = (i < chunk_depth - 1);
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();

        dungeon_floors[i] = *temp_d;

        //makes the staircases in a dungeon
        dungeon_floors[i].make_stairs(has_layer_below);
        dungeon_floors[i].make_spawner(i);
    }
    //generate the overworld
    has_layer_below = (chunk_depth > 0);
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_FOREST);
}


void Chunk::build_water_chunk() {
    chunk_depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_WATER);
}

void Chunk::build_beach_chunk() {
    chunk_depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_BEACH);
}

IntPoint Chunk::get_world_loc() const{
    return IntPoint(world_row, world_col);
}

IntPoint Chunk::get_up_stair(int depth) const{
    assert(depth>=0);
    return dungeon_floors[depth].up_stair;
}

IntPoint Chunk::get_down_stair(int depth) const{
    if(depth==-1) {
        return overworld.down_stair;
    } else {
        return dungeon_floors[depth].down_stair;
    }
}

vector<Item*>* Chunk::get_items(int depth) {
    if(depth == -1) {
        return overworld.get_items();
    } else {
        return dungeon_floors[depth].get_items();
    }
}

void Chunk::remove_item(Item* item, int depth) {
    vector<Item*>* items = get_items(depth);
    for(int i = 0; i<items->size();i++) {
        if(items->at(i) == item) {
            items->erase(items->begin() + i);
        }
    }
}

void Chunk::add_item(Item* item,  int depth) {
   if(depth == -1) {
       overworld.add_item(item);
   } else {
       dungeon_floors[depth].add_item(item);
   }
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth) {
    if (depth == -1){
        return overworld.get_ground();
    } else {
        return dungeon_floors[depth].get_dungeon();
    }
}

Tile* Chunk::get_tile(int depth, int row, int col) {
    if (depth == -1){
        return overworld.get_tile(row, col);
    } else {
        return dungeon_floors[depth].get_tile_pointer(row, col);
    }
}

void Chunk::set_tile(int depth, int row, int col, Tile tile){
    if (depth == -1) {
        overworld.set_tile(row, col, tile);
    } else {
        dungeon_floors[depth].set_tile(row, col, tile);
    }
}

int Chunk::get_depth() const {
    return chunk_depth;
}

bool Chunk::out_of_bounds(int _depth, int row, int col) const {
    return (_depth >= chunk_depth ||
            row >= height ||
            col >= width);
}

/*
 * For now, assume that there is one per floor
 * Later, I'll change this to a vector
 * But that will also have ramifications in how
 * the game will handle things.
 */
Spawner Chunk::get_spawner(int _depth) {
    if(_depth>=0) {
        return dungeon_floors[_depth].get_spawner();
    } else {
        return overworld.get_spawner();
    }
}

void Chunk::dungeon_dump(int _depth) {
    dungeon_floors[_depth].dungeon_dump();
}

MapTile Chunk::get_type()
{
    return chunk_type;
}


void Chunk::serialize() {
    if((chunk_depth < 0) || (chunk_depth > 10)) {
        cout<<"CHUNK DEPTH: "<<chunk_depth<<endl;
    }
    stringstream ss;
    ss<<"chunk"<<world_row<<"_"<<world_col;
    string file_name = std::string(CHUNK_DIR) + std::string("/") + ss.str();

    ofstream chunk_data_file(file_name.c_str(),
            std::ofstream::out | std::ofstream::binary);

    int num_header_bytes = 5;
    int bytes_per_tile = 1;

    file[0] = width;
    file[1] = height;
    file[2] = chunk_depth;
    file[3] = chunk_type.id;
    file[4] = overworld.has_layer_below;

    //THIS MUST BE CHANGED EVERY TIME THE SERIALIZATION FUNCTION IS CHANGED.
    int file_size = num_header_bytes //This refers to the bytes initialized above.
                    + 6*(chunk_depth) //Every chunk will have 6 bytes for spawner and stair locations.
                    + 2*(overworld.has_layer_below) //Stair locations in the overworld.
                    + bytes_per_tile*width*height*(chunk_depth+1); //The chunk itself.
    char file[file_size];

    int current_byte = num_header_bytes;

    if(overworld.has_layer_below) {
        file[current_byte] = overworld.down_stair.row;
        file[current_byte + 1] = overworld.down_stair.col;
        current_byte += 2;
    }

    for(int i = 0; i < chunk_depth; i++) {
        file[current_byte] = dungeon_floors[i].spawner_loc.row;
        file[current_byte + 1] = dungeon_floors[i].spawner_loc.col;
        file[current_byte + 2] = dungeon_floors[i].down_stair.row;
        file[current_byte + 3] = dungeon_floors[i].down_stair.col;
        file[current_byte + 4] = dungeon_floors[i].up_stair.row;
        file[current_byte + 5] = dungeon_floors[i].up_stair.col;
        current_byte += 6;
    }

    cout<<chunk_depth<<"  "<<current_byte<<endl;

    unsigned int tile_id, seen;
    Tile current_tile;

    for(int i = -1; i < chunk_depth; i++) {
        for(int j = 0; j < height; j++) {
            for(int k = 0; k < width; k++) {
                current_tile = *get_tile(i, j, k);
                tile_id = current_tile.tile_id;
                seen = (unsigned int) current_tile.seen;

                //Fewer than 128 different tile ids, so we can shift it to make
                //room in that byte for the boolean value! Thanks for ruining my
                //brain, x86 assembly. If the tile id is 12 and has been seen,
                //here's what it will look like:
                //
                //BEFORE: 00001100
                //AFTER: 00011001

                tile_id = tile_id << 1;
                tile_id = tile_id | seen;
                file[current_byte] = tile_id;

                current_byte += bytes_per_tile;
            }
        }
    }

    for(int i = 0; i < file_size; i++) {
        chunk_data_file<<file[i];
    }

    chunk_data_file.close();
}

