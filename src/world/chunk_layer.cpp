#include <chunk_layer.h>

ChunkLayer::ChunkLayer() {
    
}

ChunkLayer::ChunkLayer(int _width, int _height, bool _has_layer_below, bool is_dungeon) {
    width = _width;
    height = _height;
    has_layer_below = _has_layer_below;
    ground = TileMatrix(height, std::vector<Tile>(width, EMPTY));
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = 0;
    down_stair = IntPoint(0, 0);
    up_stair = IntPoint(0, 0);
    items = std::vector<Item*>();
    for(int i = 0; i < _height; i++) {
        for(int j = 0; j < _width; j++) {
            ground[i][j] = BLOCK_WALL;
        }
    }
}

ChunkLayer::ChunkLayer(const ChunkLayer& l) {
    operator=(d);
}

ChunkLayer::operator= (const ChunkLayer& l) {
    swap(d);
    return *this;
}

ChunkLayer::swap(const ChunkLayer& l) {
    dungeon = d.dungeon;
    width = d.width;
    height = d.height;
    down_stair = d.down_stair;
    up_stair = d.up_stair;
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = d.num_rooms;
    spawner = d.spawner;
    items = d.items;
    for(int i = 0; i < d.num_rooms; i++) {
        this->rooms[i] = d.rooms[i];
    }
}

ChunkLayer::tile_assertions(int row, int col) const {
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

Tile ChunkLayer::get_tile(int row, int col) {
    return ground[row][col]; 
}

Tile ChunkLayer::get_tile(IntPoint point) {
    return ground[point.row][point.col];
}

void ChunkLayer::set_tile(row, col Tile tile_type) {
    ground[row][col] = tile;
}

TileMatrix& ChunkLayer::get_ground() {
    return ground;
}

std::vector<Item*>& ChunkLayer::get_items() {
    return &items;    
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
        dungeon[down_stair.row][down_stair.col] = DOWN_STAIR;
    }
    dungeon[up_stair.row][up_stair.col] = UP_STAIR;
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
    spawner = Spawner(spawn.col, spawn.row, _depth, kobold);
    dungeon[spawn.row][spawn.col] = KOBOLD_SPAWNER;
}

void ChunkLayer::layer_dump() {
    //dungeon dump
    Tile tile;
    for(int row=0;row<height;row++)
    {
        for(int col=0;col<width;col++)
        {
            tile = dungeon[row][col];
            if(tile == DIRT) {
                cout<<".";
            } else if (tile == PATH) {
                cout<<":";
            } else if (tile == ROOM_WALL) {
                cout<<"#";
            } else if (tile == DOWN_STAIR) {
                cout<<"d";
            } else if (tile == UP_STAIR) {
                cout<<"u";
            } else if (tile == KOBOLD_SPAWNER) {
                cout<<"S";
            } else {
                cout<<" ";
            }
        }
        cout<<endl;
    }
    cout<<"---------------------------------------------"<<endl;
}
