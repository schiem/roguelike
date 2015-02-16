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
    items = std::vector<Item*>();
    spawners = std::vector<Spawner>();
    plants = std::vector<Plant>();
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
    items = std::vector<Item*>();
    spawners = std::vector<Spawner>();
    plants = std::vector<Plant>();
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

void ChunkLayer::clear() {
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms=0;
    down_stairs = std::vector<IntPoint>();
    up_stairs = std::vector<IntPoint>();
    items = std::vector<Item*>();
    spawners = std::vector<Spawner>();
    plants = std::vector<Plant>();
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            ground[i][j] = td::BLOCK_WALL;
        }
    }
}

void ChunkLayer::swap(const ChunkLayer& l) {
    ground = l.ground;
    width = l.width;
    height = l.height;
    down_stairs = l.down_stairs;
    up_stairs = l.up_stairs;
    /**
     * \todo this is weird as balls
     */
    rooms = std::vector<Room>(MAX_ROOMS, Room(IntPoint(-6, -6), IntPoint(-6, -6)));
    num_rooms = l.num_rooms;
    spawners = l.spawners;
    items = l.items;
    /// \todo There was a segfault here. This may have fixed it, but maybe not.
    plants = std::vector<Plant>(l.plants.size());
    for(int i = 0; i < l.plants.size(); i++) {
        plants[i] = l.plants[i];
    }
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

std::vector<Item*>* ChunkLayer::get_items() {
    return &items;
}

std::vector<EquipType>& ChunkLayer::get_equipment() {
    return equipment;
}

std::vector<WeaponType>& ChunkLayer::get_weapons() {
    return weapons;
}

std::vector<Spawner>* ChunkLayer::get_spawners() {
    return &spawners;
}

std::vector<Plant>* ChunkLayer::get_plants()
{
    return &plants;
}

void ChunkLayer::kill_plant(Plant* plant)
{
    for(int i=0;i<plants.size();i++)
    {
        if(plant == &plants[i])
        {
            std::vector<Misc*> temp = plant->pop_drops();
            for(int j=0;j<temp.size();j++)
            {
                add_item(temp[i]);
            }
            plants.erase(plants.begin() + i);
        }
    }
}

void ChunkLayer::add_item(Item* item) {
    items.push_back(item);
}

void ChunkLayer::make_stairs(bool has_layer_below) {
    assert(num_rooms > 0);

    Room up_room = rooms[rand() % num_rooms];
    Room down_room = rooms[rand() % num_rooms];

    IntPoint down_stair, up_stair;

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

    down_stairs.push_back(down_stair);
    up_stairs.push_back(up_stair);
}

void ChunkLayer::make_stairs_at_coords(int row, int col, Tile stair_type) {
    bool youre_a_dumbass;
    if (stair_type == td::DOWN_STAIR) {
        down_stairs.push_back(IntPoint(row, col));
    } else if (stair_type == td::UP_STAIR) {
        up_stairs.push_back(IntPoint(row, col));
    } else {
        youre_a_dumbass = true;
        assert(youre_a_dumbass);
    }
}

void ChunkLayer::make_spawner(int depth) {
    Room spawn_room;
    do {
        spawn_room = rooms[rand() % num_rooms];
    } while(spawn_room.br.row-spawn_room.tl.row<4 || spawn_room.br.col-spawn_room.tl.col<4);

    IntPoint spawn;
    do {
        //this line though
        spawn = IntPoint(2 + spawn_room.tl.row + rand() % ((spawn_room.br.row - 2) - (spawn_room.tl.row + 2)), 2 + spawn_room.tl.col + rand() % ((spawn_room.br.col - 2) - (spawn_room.tl.col + 2)));
    } while(!ground[spawn.row][spawn.col].can_build_overtop);

    spawners.push_back(Spawner(spawn.col, spawn.row, depth, enemies::kobold));
    ground[spawn.row][spawn.col] = td::KOBOLD_SPAWNER;
}

void ChunkLayer::make_spawner(int depth, IntPoint point) {
    spawners.push_back(Spawner(point.col, point.row, depth, enemies::kobold));
}

void ChunkLayer::make_spawner(int depth, IntPoint point, EnemyType enemy) {
    spawners.push_back(Spawner(point.col, point.row, depth, enemy));
}

void ChunkLayer::add_plant(Plant plant) {
    plants.push_back(plant);
}

bool ChunkLayer::in_layer(int x, int y)
{
    return y<ground.size() && y>=0 && x<ground[y].size() && x>=0;
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
