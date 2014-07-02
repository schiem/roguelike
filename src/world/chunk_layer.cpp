#include <chunk_layer.h>

namespace td=tiledef;

ChunkLayer::ChunkLayer() {

}

ChunkLayer::ChunkLayer(int _width, int _height, bool _has_layer_below) {
    width = _width;
    height = _height;
    has_layer_below = _has_layer_below;
    ground = TileMatrix(height, std::vector<Tile>(width, td::EMPTY));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    down_stair = IntPoint(0, 0);
    up_stair = IntPoint(0, 0);
    items = std::vector<Item*>();
    spawners = std::vector<Spawner>();
    for(int i = 0; i < _height; i++) {
        for(int j = 0; j < _width; j++) {
            ground[i][j] = td::BLOCK_WALL;
        }
    }
}

ChunkLayer::ChunkLayer(int _width, int _height) {
    width = _width;
    height = _height;
    ground = TileMatrix(height, std::vector<Tile>(width, td::EMPTY));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    down_stair = IntPoint(0, 0);
    up_stair = IntPoint(0, 0);
    items = std::vector<Item*>();
    spawners = std::vector<Spawner>();
    for(int i = 0; i < _height; i++) {
        for(int j = 0; j < _width; j++) {
            ground[i][j] = td::BLOCK_WALL;
        }
    }
}

ChunkLayer::ChunkLayer(const ChunkLayer& l) {
    operator=(l);
}

ChunkLayer& ChunkLayer::operator= (const ChunkLayer& l) {
    swap(l);
    return *this;
}

void ChunkLayer::swap(const ChunkLayer& l) {
    ground = l.ground;
    width = l.width;
    height = l.height;
    down_stair = l.down_stair;
    up_stair = l.up_stair;
    /**
     * \todo this is weird as balls
     */
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = l.num_rooms;
    spawners = l.spawners;
    items = l.items;
    for(int i = 0; i < l.num_rooms; i++) {
        this->rooms[i] = l.rooms[i];
    }
}

void ChunkLayer::tile_assertions(int row, int col) const {
    assert(row >= 0);
    assert(row < height);
    assert(col >= 0);
    assert(col < width);
}

Tile* ChunkLayer::get_tile_pointer(int row, int col) {
    return &ground[row][col];
}

Tile* ChunkLayer::get_tile_pointer(IntPoint point) {
    return &ground[point.row][point.col];
}

Tile ChunkLayer::get_tile(int row, int col) const {
    return ground[row][col];
}

Tile ChunkLayer::get_tile(IntPoint point) const {
    return ground[point.row][point.col];
}

void ChunkLayer::set_tile(int row, int col, Tile tile_type) {
    ground[row][col] = tile_type;
}

void ChunkLayer::set_tile(IntPoint point, Tile tile_type) {
    ground[point.row][point.col] = tile_type;
}

TileMatrix& ChunkLayer::get_ground() {
    return ground;
}

std::vector<Item*>& ChunkLayer::get_items() {
    return items;
}

std::vector<EquipType>& ChunkLayer::get_equipment() {
    return equipment;
}

std::vector<WeaponType>& ChunkLayer::get_weapons() {
    return weapons;
}

std::vector<Spawner>& ChunkLayer::get_spawners() {
    return spawners;
}

void ChunkLayer::add_item(Item* item) {
    items.push_back(item);
}

void ChunkLayer::make_stairs(bool has_layer_below) {
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
        ground[down_stair.row][down_stair.col] = td::DOWN_STAIR;
    }
    ground[up_stair.row][up_stair.col] = td::UP_STAIR;
}

void ChunkLayer::make_spawner(int depth) {
    Room spawn_room;
    do {
        spawn_room = rooms[rand() % num_rooms];
    } while(spawn_room.br.row-spawn_room.tl.row<4 || spawn_room.br.col-spawn_room.tl.col<4);

    IntPoint spawn;
    do {
        spawn = IntPoint(2 + spawn_room.tl.row + rand() % ((spawn_room.br.row - 2) - (spawn_room.tl.row + 2)), 2 + spawn_room.tl.col + rand() % ((spawn_room.br.col - 2) - (spawn_room.tl.col + 2)));
    } while(spawn==down_stair || spawn == up_stair);

    spawner_loc = spawn;
    spawners.push_back(Spawner(spawn.col, spawn.row, depth, kobold));
    ground[spawn.row][spawn.col] = td::KOBOLD_SPAWNER;
}

void ChunkLayer::make_spawner(int depth, IntPoint point) {
    spawners.push_back(Spawner(point.col, point.row, depth, kobold));
}

void ChunkLayer::layer_dump() {
    //dungeon dump
    Tile tile;
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            tile = ground[row][col];
            if(tile == td::DIRT) {
                cout<<".";
            } else if (tile == td::PATH) {
                cout<<":";
            } else if (tile == td::ROOM_WALL) {
                cout<<"#";
            } else if (tile == td::DOWN_STAIR) {
                cout<<"d";
            } else if (tile == td::UP_STAIR) {
                cout<<"u";
            } else if (tile == td::KOBOLD_SPAWNER) {
                cout<<"S";
            } else {
                cout<<" ";
            }
        }
        cout<<endl;
    }
    cout<<"---------------------------------------------"<<endl;
}
