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

#ifdef ROOM_COUNT_DEBUG
        cout<<"DUNGEONBUILDER: "<<db.num_rooms<<endl;
        cout<<"(dungeon): "<<db.get_dungeon()->num_rooms<<endl;
        for(int j = 0; j < db.num_rooms; j++) {
            cout<<j<<": "<<db.get_dungeon()->rooms[j].tl.col
                <<", "<<db.get_dungeon()->rooms[j].tl.row<<endl;
        }
        cout<<endl;

        cout<<"TEMP_D: "<<temp_d->num_rooms<<endl;
        for(int j = 0; j < temp_d->num_rooms; j++) {
            cout<<j<<": "<<temp_d->rooms[j].tl.col<<", "
                <<temp_d->rooms[j].tl.row<<endl;
        }
        cout<<"DUNGEON1: "<<dungeon_floors[i].num_rooms<<endl;
        for(int j = 0; j < dungeon_floors[i].num_rooms; j++) {
            cout<<j<<": "<<dungeon_floors[i].rooms[j].tl.col
                <<", "<<dungeon_floors[i].rooms[j].tl.row<<endl;
        }
#endif

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

int Chunk::get_x() const {
    return x;
}

int Chunk::get_y() const {
    return y;
}
bool Chunk::is_initialized() const {
    return initialized;
}
