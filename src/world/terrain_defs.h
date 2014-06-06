/**
 *  TERRAIN_DEFS.H
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
    extern Tile TILE_INDEX[24]; //CHANGE THIS WHEN YOU ADD TILES!

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
    extern Tile SAND1;
    extern Tile SAND2;
    extern Tile BIG_TREE;
    extern Tile GRASS_DIRT;
    extern Tile KOBOLD_CORPSE;
    extern Tile BOOTS;
}

struct MapTile {
    int char_count;
    int color;
    int id;
    bool does_spawn;
    bool operator==(const MapTile& rhs) const {
        return this->id == rhs.id;
    }
    bool operator!=(const MapTile& rhs) const {
        return !(this->id == rhs.id);
    }
};

namespace map_tile {
    extern MapTile MAP_TILE_INDEX[5];

    extern MapTile MAP_DEFAULT;
    extern MapTile MAP_WATER;
    extern MapTile CURSOR;
    extern MapTile MAP_FOREST;
    extern MapTile MAP_BEACH;
}

#endif
