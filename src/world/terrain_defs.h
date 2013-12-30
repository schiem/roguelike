#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H
#include <def.h>

struct Tile {
    int char_count;
    int tile_id;
    int color;
    bool can_be_moved_through;
    bool visible;
    bool opaque;
    bool seen;
    bool operator==(const Tile& rhs) const {
        return this->tile_id == rhs.tile_id;
    }
};

namespace tiledef {
    extern Tile OVERWORLD_DIRT;
    extern Tile DIRT;
    extern Tile DUNGEON_BORDER;
    extern Tile ROOM_WALL;
    extern Tile EMPTY;
    //static const Tile PATH = {88, 'X', true};
    //static const Tile PATH = {111, 'o', true};
    extern Tile PATH;
    extern Tile MAIN_CHAR;
    extern Tile MAIN_CHAR2;
    extern Tile MAIN_CHAR3;
    extern Tile MAIN_CHAR4;
    extern Tile TREE;
    extern Tile BLOCK_WALL;
    extern Tile DOWN_STAIR;
    extern Tile UP_STAIR;
    extern Tile KOBOLD;
    extern Tile KOBOLD_SPAWNER;
    extern Tile WATER;
    extern Tile LIGHT_WATER;
}

struct MapTile {
    int char_count;
    int color;
    bool operator==(const MapTile& rhs) const {
        return this->char_count == rhs.char_count;
    }
};

namespace map_tile {
    extern MapTile MAP_DEFAULT;
    extern MapTile MAP_WATER;
    extern MapTile CURSOR;
}

#endif
