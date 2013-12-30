#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    initialized = false;
}

Chunk::Chunk(int _width, int _height, MapTile tile_type) {
    initialized = true;
    width = _width;
    height= _height;

    if(tile_type == map_tile::MAP_DEFAULT) {
        build_land_chunk();
    } else if (tile_type == map_tile::MAP_WATER) {
        build_water_chunk();
    } else if (tile_type == map_tile::MAP_BEACH) {
        build_beach_chunk();
    }
}

void Chunk::build_land_chunk() {
    depth = rand() % 6 + 1;
    dungeon_floors = vector<Dungeon>(depth, Dungeon(width, height));
    Dungeon* temp_d;
    ProcedurallyBlindDB db(width, height);
    //CorruptiblePBlindDB db(width, height);

    bool has_layer_below;

    for (int i=0; i < depth; i++) {
        has_layer_below = (i < depth - 1);
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();

        dungeon_floors[i] = *temp_d;

        //makes the staircases in a dungeon
        dungeon_floors[i].make_stairs(has_layer_below);
        dungeon_floors[i].make_spawner(i);
    }
    //generate the overworld
    has_layer_below = (depth > 0);
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_DEFAULT);
}

void Chunk::build_water_chunk() {
    depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_WATER);
}

void Chunk::build_beach_chunk() {
    depth = 0;
    bool has_layer_below = false;
    overworld = Overworld(width, height, has_layer_below, map_tile::MAP_BEACH);
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

void Chunk::set_tile(int depth, int row, int col, Tile* tile){
    if (depth == -1) {
        overworld.set_tile(row, col, tile);
    } else {
        dungeon_floors[depth].set_tile(row, col, *tile);
    }
}

IntPoint Chunk::get_up_stair(int depth) const{
    assert(depth>=0);
    return dungeon_floors[depth].up_stair;
}

IntPoint Chunk::get_down_stair(int depth) const{
    if(depth==-1)
    {
        return overworld.down_stair;
    }
    else
    {
        return dungeon_floors[depth].down_stair;
    }
}

int Chunk::get_depth() const {
    return depth;
}

bool Chunk::is_initialized() const {
    return initialized;
}

bool Chunk::out_of_bounds(int _depth, int row, int col) const {
    return (_depth >= depth ||
            row >= height ||
            col >= width);
}

/*
 * For now, assume that there is one per floor
 * Later, I'll change this to a vector
 * But that will also have ramifications in how
 * the game will handle things.
 */

Spawner Chunk::get_spawner(int _depth)
{
    if(_depth>=0)
    {
        return dungeon_floors[_depth].get_spawner();
    }
    else
    {
        return overworld.get_spawner();
    }
}

void Chunk::dungeon_dump(int _depth)
{
    dungeon_floors[_depth].dungeon_dump();
}

