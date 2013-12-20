#include <chunk.h>
using namespace std;

Chunk::Chunk() {
    initialized = false;
}

Chunk::Chunk(int _y, int _x, int _width, int _height) {
    initialized = true;
    width = _width;
    height= _height;
    bool is_dungeon;
    srand(time(NULL));
    depth = rand() % 6 + 1;
    dungeon_floors = vector<Dungeon>(depth, Dungeon(width, height));
    x = _x;
    y = _y;
    Dungeon* temp_d;
    ProcedurallyBlindDB db(width, height); 
    //CorruptiblePBlindDB db(width, height);
    for (int i=0; i < depth; i++) {
        is_dungeon = (i < depth - 1);
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();
        dungeon_floors[i] = *temp_d;
        //makes the staircases in a dungeon
        dungeon_floors[i].make_stairs(is_dungeon);
    }
    //generate the overworld
    is_dungeon = (depth > 0);
    overworld = Overworld(width, height, is_dungeon);
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth) {
    if (depth == -1){
        return overworld.get_ground();
    } else {
        return dungeon_floors[depth].get_dungeon();
    }
}

Tile Chunk::get_tile(int depth, int col, int row) const {
    if (depth == -1){
        return overworld.get_tile(col, row);
    } else {
        return dungeon_floors[depth].get_tile(col, row);
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

int Chunk::get_x() const {
    return x;
}

int Chunk::get_y() const {
    return y;
}
bool Chunk::is_initialized() const {
    return initialized;
}

