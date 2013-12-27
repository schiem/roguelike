#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    initialized = false;
}

Chunk::Chunk(int _width, int _height) {
    initialized = true;
    width = _width;
    height= _height;
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
        //dungeon_floors[i].make_border();
    }
    //generate the overworld
    has_layer_below = (depth > 0);
    overworld = Overworld(width, height, has_layer_below);
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

std::vector<int> Chunk::get_up_stair(int depth) const{
    assert(depth>=0);
    return dungeon_floors[depth].up_stair;
}

std::vector<int> Chunk::get_down_stair(int depth) const{
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
