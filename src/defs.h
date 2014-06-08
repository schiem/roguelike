/**
 *  DEFS.H
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

#ifndef DEFS_H
#define DEFS_H
#include <color_def.h>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <vector>

enum STATS
{
    HEALTH,
    ARMOR,
    ATTACK
};


/****************************
 *   TERRAIN DEFS
 ***************************/

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
    extern Tile TILE_INDEX[29]; //CHANGE THIS WHEN YOU ADD TILES!
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
    extern Tile RABBIT;
    extern Tile RABBIT_CORPSE;
    extern Tile SWORD;
    extern Tile POTATO;
    extern Tile PLANT;
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

/****************************
 *   ITEM DEFS
 ***************************/
/*****
     0
   54145
     1
    2 2
    3 3
 Body index diagram.
******/

struct EquipType
{
    float weight;
    Tile sprite;
    std::string name;
    std::string description;
    int rarity;
    int body_part;
    int type;
    int armor_class;
    bool use;
    bool operator==(const EquipType& rhs) const 
    {
        return this->name==rhs.name;
    }
};

struct WeaponType
{
    float weight;
    Tile sprite;
    std::string name;
    std::string description;
    int rarity;
    int damage;
    int type;
    int range;
    bool use;
    bool operator==(const EquipType& rhs) const 
    {
        return this->name==rhs.name;
    }
};

struct ConsumableType
{
    float weight;
    Tile sprite;
    std::string name;
    std::string description;
    bool use;
    int rarity;
    int stat;
    int value;
    int type;
    bool operator==(const ConsumableType& rhs) const 
    {
        return this->name==rhs.name;
    }
};

struct MiscType
{
    float weight;
    Tile sprite;
    std::string name;
    std::string description;
    bool use;
    int rarity;
    bool operator==(const MiscType& rhs) const
    {
        return this->name==rhs.name;
    }
};


namespace equipment
{
    extern EquipType boots;
}

namespace weapons
{
    extern WeaponType dagger;
}

namespace consumables
{
    extern ConsumableType potato;
    static const int NUM_CONSUMABLES = 1;
    extern ConsumableType CONSUMABLE_LIST[NUM_CONSUMABLES];
    enum ConsumableTypes
    {
        RESTORE,
        FORTIFY,
        DROP,
        INCREASE
    };
}

namespace misc
{
    extern MiscType kobold_corpse;
    extern MiscType rabbit_corpse;
    extern MiscType player_corpse;
}



/****************************
 *   ENEMY DEFS
 ***************************/

struct EnemyType
{
    int moral;
    int max_health;
    int base_attack;
    int armor;
    int id;
    int sight;
    int speed;
    std::string name;
    Tile sprite;
    MiscType corpse;
    std::vector<EquipType> eq;
    std::vector<WeaponType> wep;
    bool operator==(const EnemyType& rhs) const
    {
        return this->id==rhs.id;
    }
};



namespace enemies
{
    extern EnemyType kobold;
    extern EnemyType rabbit;
    extern EnemyType ENEMY_LIST[2];
    extern int NUM_ENEMIES;
}

#endif


