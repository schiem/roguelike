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

#include <iostream>
#include <procedurally_blind_db.h>
#include <corruptible_pblind_db.h>
#include <overworld_gen.h>
#include <spawner.h>
#include <plant.h>
#include <int_point.h>

#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    cm.height = CHUNK_HEIGHT;
    cm.width = CHUNK_WIDTH;
}

Chunk::Chunk(MapTile tile_type, int world_row, int world_col, string _save_folder) {
    cm.height = CHUNK_HEIGHT;
    cm.width = CHUNK_WIDTH;
    init(tile_type, world_row, world_col, _save_folder);
}

bool Chunk::find_serialized_chunk() {
    fs::path chunk_dir(save_folder);

    stringstream ss;
    ss << save_folder << "/chunk" << cm.world_row << "_" <<cm.world_col;
    string filename = ss.str();

    if(fs::exists(chunk_dir) && fs::is_directory(chunk_dir)) {
        if(fs::exists(filename)) {
            deserialize(filename);
            return true;
        }
    } else {
        cout<<"Chunk directory is missing. Aborting."<<endl;
        exit(EXIT_FAILURE);
    }

    return false;
}

void Chunk::init(MapTile tile_type, int world_row, int world_col, string _save_folder) {
    save_folder = _save_folder;
    chunk_type = tile_type;
    cm.world_row = world_row;
    cm.world_col = world_col;

    bool found_chunk = find_serialized_chunk();
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

bool Chunk::build_chunk_with_dungeons() {
    cm.depth = rand() % 6 + 1;
    layers = std::vector<ChunkLayer>(cm.depth, ChunkLayer(cm.width, cm.height));
    //CorruptiblePBlindDB db(cm.width, cm.height);

    bool has_layer_below;
    //Don't generate the top layer; it will be an overworld.
    for (int i=1; i < cm.depth; i++) {
        has_layer_below = (i < cm.depth - 1);
        pblind_db::build_dungeon(cm.width, cm.height, 5, layers[i]);

        //makes the staircases in a dungeon
        layers[i].make_stairs(has_layer_below);
        //layers[i].make_spawner(i);
        layers[i].has_layer_below = has_layer_below;
    }
}

void Chunk::build_land_chunk() {
    build_chunk_with_dungeons();
    overworld_gen::build_land_overworld(layers[0]);
}

void Chunk::build_forest_chunk() {
    build_chunk_with_dungeons();
    overworld_gen::build_forest_overworld(layers[0]);
    
    //TODO: probably rename this
    build_some_dank_trees();
}


void Chunk::build_water_chunk() {
    cm.depth = 1;
    layers = std::vector<ChunkLayer>(1, ChunkLayer(cm.width, cm.height));
    overworld_gen::build_water_overworld(layers[0]);
}

void Chunk::build_beach_chunk() {
    cm.depth = 1;
    layers = std::vector<ChunkLayer>(1, ChunkLayer(cm.width, cm.height));
    overworld_gen::build_beach_overworld(layers[0]);
}


void Chunk::build_some_dank_trees()
{
    //erm...let's have a tree density
    //(that's trees/tile sq)
    float TREE_DENSITY = .01;
    int num_trees = (cm.width * cm.height) * TREE_DENSITY;
    for(int i=0;i<num_trees;i++)
    {
        int x;
        int y;
        do {
            x = rand() % cm.width;
            y = rand() % cm.height;
        } while(can_build(x, y) == false);
        layers[0].add_plant(Plant(x, y, plants::tree));
    }
}

/**
 * MAKE THIS GENERALIZED FOR ALL CHUNKS.
 */
bool Chunk::can_build(int x, int y)
{
    //Should we even be putting a tree here?
    bool good_tile = layers[0].get_tile(y, x).can_be_moved_through;
    
    //Is it in a spawner?
    bool in_spawner = false;
    std::vector<Spawner>* spawners = get_spawners(0);
    for(int i=0;i<spawners->size();i++)
    {
        if(spawners->at(i).point_in_spawner(x, y))
        {
            in_spawner = true;
        }
    }

    //Is it on the stairs?
    IntPoint stair = get_down_stair(0);
    bool is_stair = (x == stair.col) && (y == stair.row);
    
    return good_tile && !in_spawner && !is_stair;
}

IntPoint Chunk::get_world_loc() const{
    return IntPoint(cm.world_row, cm.world_col);
}

IntPoint Chunk::get_up_stair(int depth) const{
    assert(depth>0);
    assert(cm.depth > depth);
    return layers[depth].up_stair;
}

IntPoint Chunk::get_down_stair(int depth) const{
    assert(cm.depth > depth);
    return layers[depth].down_stair;
}

std::vector<Item*>* Chunk::get_items(int depth) {
    return layers[depth].get_items();
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
    assert(depth < cm.depth);
    layers[depth].add_item(item);
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth) {
    return layers[depth].get_ground();
}

Tile* Chunk::get_tile(int depth, int row, int col) {
    assert(!out_of_bounds(depth, row, col));
    return layers[depth].get_tile_pointer(row, col);
}

void Chunk::set_tile(int depth, int row, int col, Tile tile){
    layers[depth].set_tile(row, col, tile);
}

int Chunk::get_depth() const {
    return cm.depth;
}

bool Chunk::out_of_bounds(int _depth, int row, int col) const {
    return (_depth >= cm.depth ||
            row >= cm.height ||
            col >= cm.width ||
            _depth < 0 ||
            row < 0 ||
            col < 0);
}

/*
 * For now, assume that there is one per floor
 * Later, I'll change this to a vector
 * But that will also have ramifications in how
 * the game will handle things.
 */
std::vector<Spawner>* Chunk::get_spawners(int _depth) {
    ChunkLayer* current = &layers[_depth];
    return current->get_spawners();
}

std::vector<Plant>* Chunk::get_plants(int _depth) {
    ChunkLayer* current = &layers[_depth];
    return current->get_plants();
}

void Chunk::dungeon_dump(int _depth) {
    layers[_depth].layer_dump();
}

MapTile Chunk::get_type() {
    return chunk_type;
}

//===========SERIALIZATION/DESERIALIZATION=========

int Chunk::calculate_file_size(int bytes_per_tile) {
    int bytes_per_layer = 0;
    for(int i = 0; i < cm.depth; i++) {
        for(int j = 0; j < layers[i].spawners.size(); j++) {
            bytes_per_layer += 3;
        }
        bytes_per_layer += 5;
    }

    bytes_per_layer += (BYTES_PER_TILE * cm.width * cm.height * cm.depth);

    return (CHUNK_META_BYTES + bytes_per_layer);
}

int Chunk::serialize_metadata(char* file) {
    file[0] = cm.width;
    file[1] = cm.height;
    file[2] = cm.depth;
    file[3] = cm.chunk_type_id;
    file[4] = cm.world_row;
    file[5] = cm.world_col;

    return 6;
}

int Chunk::serialize_layer_metadata(char file[], int cb) {
    int current_byte=cb;

    for(int i = 0; i < cm.depth; i++) {
        file[current_byte + 0] = layers[i].spawners.size();
        current_byte += 1;
        for(int j = 0; j < layers[i].spawners.size(); j++) {
            file[current_byte + 0] = layers[i].spawners[j].get_y();
            file[current_byte + 1] = layers[i].spawners[j].get_x();
            file[current_byte + 2] = layers[i].spawners[j].get_enemy_type_id();
            current_byte += 3;
        }
        file[current_byte + 0] = layers[i].down_stair.row;
        file[current_byte + 1] = layers[i].down_stair.col;
        file[current_byte + 2] = layers[i].up_stair.row;
        file[current_byte + 3] = layers[i].up_stair.col;
        current_byte += 4;
    }

    return current_byte;
}

int Chunk::serialize_layers(char file[], int cb) {
    int current_byte=cb;

    char tile_id, seen;
    Tile current_tile;

    for(int i = 0; i < cm.depth; i++) {
        for(int j = 0; j < cm.height; j++) {
            for(int k = 0; k < cm.width; k++) {
                current_tile = *get_tile(i, j, k);
                tile_id = (char) current_tile.tile_id;
                seen = (char) current_tile.seen;

                file[current_byte] = tile_id;
                file[current_byte+1] = seen;

                current_byte += BYTES_PER_TILE;
            }
        }
    }

    return current_byte;
}


void Chunk::save_file(char file[], string filename, int file_size) {
    string full_path = save_folder + std::string("/") + filename;
    ofstream chunk_data_file;

    //TODO Valgrind complains about the following line.
    chunk_data_file.open(full_path.c_str(), std::ofstream::out | std::ofstream::binary);

    for(int i = 0; i < file_size; i++) {
        //TODO Valgrind complains about the following line.
        chunk_data_file<<file[i];
    }

    chunk_data_file.close();
}


void Chunk::serialize() {
    stringstream ss;
    ss<<"chunk"<<cm.world_row<<"_"<<cm.world_col;

    int file_size = calculate_file_size(BYTES_PER_TILE);

    //Set up the file array to write to
    char file[file_size];
    int current_byte = serialize_metadata(file);
    current_byte = serialize_layer_metadata(file, current_byte);
    current_byte = serialize_layers(file, current_byte);
    assert(current_byte == file_size);

    save_file(file, ss.str(), file_size);
}

void Chunk::deserialize_metadata(char file_data[]) {
    cm.width = file_data[0];
    cm.height = file_data[1];
    cm.depth = file_data[2];
    cm.chunk_type_id = file_data[3];
    cm.world_row = file_data[4];
    cm.world_col = file_data[5];

    /*
     * The map tile ID was stored, now we just reverse it to get the chunk type.
     */
    chunk_type = map_tile::MAP_TILE_INDEX[file_data[3]];
    //TODO return 6
}

int Chunk::deserialize_layer_metadata(char file_data[], int cb) {
    char num_spawners, spawner_row, spawner_col;
    EnemyType enemy_type;
    int current_byte = cb;

    for(int i = 0; i < cm.depth; i++) {
        num_spawners = file_data[current_byte + 0];
        current_byte += 1;

        for(int j = 0; j < num_spawners; j++) {
            spawner_row = file_data[current_byte + 0];
            spawner_col = file_data[current_byte + 1];
            enemy_type = enemies::ENEMY_LIST[file_data[current_byte+2]];
            layers[i].make_spawner(i, IntPoint(spawner_row, spawner_col), enemy_type);
            current_byte += 3;
        }

        layers[i].down_stair.row = file_data[current_byte + 0];
        layers[i].down_stair.col = file_data[current_byte + 1];
        layers[i].up_stair.row = file_data[current_byte + 2];
        layers[i].up_stair.col = file_data[current_byte + 3];
        layers[i].has_layer_below = (i < (cm.depth - 1));
        current_byte += 4;
    }

    return current_byte;
}

int Chunk::deserialize_layers(char file_data[], int cb) {
    int current_byte = cb;

    Tile current_tile;
    unsigned int tile_id;
    bool seen;
    //Undo the serialization.
    for(int i = 0; i < cm.depth; i++) {
        for(int j = 0; j < cm.height; j++) {
            for(int k = 0; k < cm.width; k++) {
                tile_id = (file_data[current_byte]);
                seen = file_data[current_byte + 1];
                current_tile=tiledef::TILE_INDEX[tile_id];
                current_tile.seen = seen;

                set_tile(i, j, k, current_tile);
                current_byte += BYTES_PER_TILE;
            }
        }
    }

    return current_byte;
}

void Chunk::deserialize(string file_name) {
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

    deserialize_metadata(file_data);

    int current_byte=CHUNK_META_BYTES;
    layers = vector<ChunkLayer>(cm.depth, ChunkLayer(cm.width, cm.height));

    current_byte = deserialize_layer_metadata(file_data, current_byte);
    //assert(cm.depth == (current_byte - 4) / 6); Unfortunately it's not this simple anymore.

    current_byte = deserialize_layers(file_data, current_byte);
    assert(current_byte == file_size);

    //Cleaning up.
    delete [] file_data;
    chunk_data_file.close();
}

