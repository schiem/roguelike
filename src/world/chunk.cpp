#include <chunk.h>
using namespace std;

Chunk::Chunk(){
    initialized = false;
}

Chunk::Chunk(int _y, int _x, int _width, int _height)
{
    initialized = true;
    width = _width;
    height= _height;
    //srand(time(NULL));
    depth = rand() % 6;
    //dungeon = new Dungeon[depth];
    dungeon_floors = vector<Dungeon>(depth, Dungeon(width, height));
    x = _x;
    y = _y;
    Dungeon* temp_d;
    //ProcedurallyBlindDB db(width, height); 
    CorruptiblePBlindDB db(width, height);

    for (int i=0; i < depth; i++){
        db.build_dungeon(5, 5);
        temp_d = db.get_dungeon();
        dungeon_floors[i] = *temp_d;
    }
    //generate the overworld
    overworld = Overworld(width, height);
}

/*
Chunk::~Chunk(){
    delete []dungeon;
}
*/

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth){
    if (depth == -1){
        return overworld.get_ground();
    }
    else{
        return dungeon_floors[depth].get_dungeon();
    }
}

Tile Chunk::get_tile(int depth, int col, int row) const
{
    if (depth ==-1){
        return overworld.get_tile(col, row);
    }
    else{
        return dungeon_floors[depth].get_tile(col, row);
    }
}

int Chunk::get_depth() const
{
    return depth;
}

int Chunk::get_x() const
{
    return x;
}

int Chunk::get_y() const
{
    return y;
}
bool Chunk::is_initialized() const
{
    return initialized;
}

