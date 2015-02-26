/**
 *  @file DEFS.H
 *  @author Michael Yoder, Seth Yoder
 *
 *
 *  @section LICENSE
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

//Let the record show, that Alan, Jack, and I think that Google runs thepiratebay.se
//6/7/2014

#ifndef DEFS_H
#define DEFS_H
#include <color_def.h>
#include <string>
#include <ctime>
#include <stdlib.h>
#include <vector>
#define DEBUG 1
//Why is this a define?
#define SAVE_DIR DATADIR "/save"

/**
 * Holds the different stats of the character.
 * The enum acts as an index in the stats and current stats,
 * so current_stats[HEALTH] corresponds to character's current
 * health.
 * @todo Why on earth is this in the defs and not the character?!
 * @see Character::current_stats, Character::stats
 */
enum STATS
{
    HEALTH,
    ATTACK,
    ENDURANCE,
    EXPERIENCE,
    LEVEL,
    STRENGTH,
    DEXTERITY,
    INTELLIGENCE
};

const int NUM_STATS = 8;

extern std::string STAT_NAMES[NUM_STATS]; 

enum ITEM_CATEGORY
{
    AXE,
    LONG_BLADE,
    SHORT_BLADE,
    BOW,
    ARMOR,
    INGREDIENT,
    USELESS
};

enum MESSAGES
{
    ATTACK_SELF,
    ATTACK_OTHER,
    HARVEST_PLANT,
    KILL_OTHER
};

const int NUM_MESSAGES = 4;

extern std::vector<std::vector<std::string> > MESSAGE_LIST;

/*--------------------------
 *   TERRAIN DEFS
 -------------------------*/

/**
 * Struct to keep track of the information stored in the terrain tiles.
 * This keeps track of the tiles that make up the entire game's graphical
 * system.
 */
struct Tile {
    /**
     * The number of the tile corresponding to a character mapping.
     * The character of the tile is generated by ascii_lib which is
     * included in the lib/ folder.
     */
    int char_count;

    /**
     * A sequential id assigned to tiles for accessing them.
     */
    int tile_id;

     /**
     * The color of the tile.
     * An integer which corresponds to the color of the tile.  It is a 24 bit
     * binary number where each 8 bits represents, R, G, or B which has been
     * converted into decimal.
     */
    int color;

    /**
     * Whether or not a character can move through the tile.
     */
    bool can_be_moved_through;

    /**
     * If the tile is currently visible.
     * Tiles will be marked as visible if they can currently be seen by the
     * main character.  Otherwise, this will be false.
     */
    bool visible;

    /**
     * True if light can pass through the tile.
     * This value is used by the lighting system to determine whether or not
     * a particular tile should have light pass through it.
     */
    bool opaque;

    /**
     * True if the main character has, at some point, seen the tile.
     * If this is true, then the tile will be grayed out when it is
     * not visible.  Otherwise, a solid black rectangle will be rendered
     * instead of the tile.
     */
    bool seen;

    /**
     * True if this tile is at the floor level - this is used to determine if a
     * spawner or stairs can be placed here.
     */
    bool can_build_overtop;

    /**
     * The comparison operator for tiles.
     */
    bool operator==(const Tile& rhs) const {
        return this->tile_id == rhs.tile_id;
    }
    
    bool operator!=(const Tile& rhs) const{
        return this->tile_id != rhs.tile_id;
    }
};

namespace tiledef {
    static const int TILE_TYPE_COUNT = 37;
    extern Tile TILE_INDEX[TILE_TYPE_COUNT]; //CHANGE THIS WHEN YOU ADD TILES!
    extern Tile OVERWORLD_DIRT; //YOU'RE NOT MY MOM!
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
    extern Tile WOOD_WALL;
    extern Tile WOOD_FLOOR;
    extern Tile DOOR;
    extern Tile BURROW;
    extern Tile HUT_WALL;
    extern Tile AXE;
    extern Tile LOG;
    extern Tile WOLF;
}

/**
 * The definition for the tiles used to construct the world map.
 */
struct MapTile {
    /**
     * The number of the tile corresponding to a character mapping.
     * The character of the tile is generated by ascii_lib which is
     * included in the lib/ folder.
     */
    int char_count;

    /**
     * The color of the tile.
     * An integer which corresponds to the color of the tile.  It is a 24 bit
     * binary number where each 8 bits represents, R, G, or B which has been
     * converted into decimal.
     */
    int color;

    /**
     * A sequential id assigned to tiles for accessing them.
     */
    int id;

    /**
     * True if the tile spawns any mobs.
     * \todo Change this so that it is a list of the mobs to spawn
     */
    bool does_spawn;

    /**
     * What kind of blending does this tile have?  See the corresponding
     * enum called BLENDTYPES for this.
     */
     int blend_type;

    /**
     * The base tile of the chunk.
     */
     Tile base_tile;

    /**
     * Comparison operator for MapTiles.
     */
    bool operator==(const MapTile& rhs) const {
        return this->id == rhs.id;
    }

    /**
     * != operator for MapTiles.
     */
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
    enum BLENDTYPES
    {
        HARD,
        NORMAL
    };
}

/*--------------------------
 -       ITEM DEFS
 -------------------------*/
/**
 * The definition for the different types of equipment.
 * This defines the various attributes of equipment, and members of
 * this struct are used to construct equipment items.
 * @see Equipment
 */

struct EquipType
{
    /**
     * How much the item weighs.
     */
    float weight;

    /**
     * The tile which will be used to graphically represent the item.
     */
    Tile sprite;

    /**
     * The name of the item.
     */
    std::string name;

    /**
     * A description of the item.
     */
    std::string description;

    /**
     * How likely it is tha the item will appear.
     */
    int rarity;

    /**
     * The body part which the equipment goes on.
     * The indexes correspond as follows:
     * 0 - head
     * 1 - torso
     * 2 - legs
     * 3 - feet
     * 4 - arms
     * 5 - hands
     * 6 - weapon
     * \todo Put this in an enum.
     */
    int body_part;

    /**
     * The type of equipment.
     * The different types of equipment are as follows:
     * 0 - light
     * 1 - medium
     * 3 - heavy.
     * Each type of armor has a type ofweapon that will deal more
     * damage against it.
     * \todo Put this in an enum.
     */
    int type;

    /**
     * How much the armor protects against getting hit.
     * Each value corresponds to a differen type of weapon.
     * This will be some number.  Each weapon will have a threshold.
     * If the hit number is higher than the threshold, then the weapon
     * will be completely blocked.  Hmm...that seems a bit OP.  Maybe
     * I should change that.
     */
    int hit[3];

    /**
     * How much the armor protects against damage.
     * Each value corresponds to a different type of weapon.
     * This will be some a percentage of damage reduction.
     */
    float dam[3];

    /**
     * True if the item has a use beyond increasing armor.
     */
    bool use;

    /**
     * The size of the item.
     */
    int size;

    /**
     * The category the item belongs to.
     */
    int category;

    /**
     * Comparitor operator for equipment.
     */
    bool operator==(const EquipType& rhs) const
    {
        return this->name==rhs.name;
    }
};

/**
 * The definition for the different types of weapon.
 * This defines the various attributes of weapon, and members of
 * this struct are used to construct weapon items.
 * @see Weapon
 */
struct WeaponType
{
    /**
     * How much the item weighs.
     */
    float weight;

    /**
     * The tile which will be used to graphically represent the item.
     */
    Tile sprite;

    /**
     * The name of the item.
     */
    std::string name;

    /**
     * A description of the item.
     */
    std::string description;

    /**
     * how likely it is tha the item will appear.
     */
    int rarity;

    /**
     * The amount of damage the weapon will deal when equipped.
     */
    int damage;

    /**
     * The type of weapons.
     * The different types of weapons are as follows:
     * 0 - piercing
     * 1 - slashing
     * 3 - bludgeoning.
     * Each type of armor has a type of weapon that will deal more
     * damage against it.
     * \todo Put this in an enum.
     */
    int type;



    /**
     * How far the weapon can be used.
     * \todo Make this do something.
     */
    int range;

    /**
     * True if the item has a use beyond changing attack.
     */
    bool use;
    
    /**
     * The size of the item.
     */
    int size;

    /**
     * The category the item belongs to.
     */
    int category;
    
    /**
     * Comparitor operator for weapon.
     */
    bool operator==(const WeaponType& rhs) const
    {
        return this->name==rhs.name;
    }
};

/**
 * The definition for the different types of consumable.
 * This defines the various attributes of consumable, and members of
 * this struct are used to construct consumable items.
 * @see Consumable
 */
struct ConsumableType
{
    /**
     * How much the item weighs.
     */
    float weight;

    /**
     * The tile which will be used to graphically represent the item.
     */
    Tile sprite;

    /**
     * The name of the item.
     */
    std::string name;

    /**
     * A description of the item.
     */
    std::string description;

    /**
     * True if the item has a use beyond changing attack.
     */
    bool use;

    /**
     * How likely it is tha the item will appear.
     */
    int rarity;

    /**
     * The stat that the item modifies.
     * This corresponds to an index in the character's stats
     * and current_stats array.
     * @see Character
     * @see STATS
     */
    int stat;

    /**
     * How much the stat is being modified by.
     */
    int value;

    /**
     * The type of modification the stat will perform.
     * This corresponds to an enum containing the different
     * types of a consumables.
     * @see ConsumableTypes
     */
    int type;

    /**
     * The size of the item.
     */
    int size;
    
    /**
     * The category the item belongs to.
     */
    int category;

    /**
     * Comparitor for consumables.
     */
    bool operator==(const ConsumableType& rhs) const
    {
        return this->name==rhs.name;
    }
};

/**
 * The definition for the different types of misc items.
 * This defines the various attributes of misc items, and members of
 * this struct are used to construct misc items.
 * @see Misc
 */
struct MiscType
{
     /**
     * How much the item weighs.
     */
    float weight;

    /**
     * The tile which will be used to graphically represent the item.
     */
    Tile sprite;

    /**
     * The name of the item.
     */
    std::string name;

    /**
     * A description of the item.
     */
    std::string description;

    /**
     * True if the item has a use beyond changing attack.
     */
    bool use;

    /**
     * How likely it is tha the item will appear.
     */
    int rarity;

    /**
     * The size of the item.
     */
    int size;

    /**
     * The category the item belongs to.
     */
    int category;
    
    /**
     * Comparitor for the MiscType.
     */
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
    extern WeaponType wood_axe;
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
    extern MiscType wood;
}



/*--------------------------
 *   ENEMY DEFS
 -------------------------*/

/**
 * Definition for constructing a spawner.
 */

struct SpawnType
{
    /**
     * Determines whether everything should spawn at once, or
     * over time.
     */
    bool spawn_immediately;

    /**
     * The minimum number of enemies to spawn.
     */
    int min_enemies;
    
    /**
     * The maximum number of enemies to spawn.
     */
    int max_enemies;

    /**
     * The number of enemies per point of spawning (how many enemies/
     * hut, or whatever.
     */
    int enemies_per_spawn;

    /**
     * The type of den to create for the critters.
     */
    int den_type;
};

namespace spawners
{
    extern SpawnType rabbit;
    extern SpawnType kobold;
    enum Dens
    {
        BURROW,
        HUTS
    };
}

/**
 * The definition for the different types of enemies.
 * This defines the various attributes of enemies, and members of
 * this struct are used to construct enemies.
 * @see Enemy
 */
struct EnemyType
{
    /**
     * The attribute to assign the member variable moral for enemies.
     */
    int moral;
     
    /**
     * The attributes to assign the member variables.
     */
    std::vector<int> stats;

    /**
     * The attribute to assign the member variable id for enemies.
     */
    int id;

    /**
     * The attribute to assign the member variable ai id for enemies.
     */
    int ai_id;

    /**
     * The attribute to assign the member variable sight for enemies.
     */
    int sight;

    /**
     * The attribute to asisgn to the member variable view for enemies.
     */
    int view;

    /**
     * The attribute to assign the member variable speed for enemies.
     */
    int speed;

     /**
     * The attribute to assign the member variable name for enemies.
     */
    std::string name;

     /**
     * The attribute to assign the member variable sprite for enemies.
     */
    Tile sprite;

     /**
     * The attribute used to create the member varaible corpse for enemies.
     */
    MiscType corpse;

    /**
     * A list of possible equipment that enemies can have.
     * @see Enemy::generate_equipment()
     */
    std::vector<EquipType> eq;

    /**
     * A list of possible weapons that enemies can have.
     * @see Enemy::generate_weapon()
     */
    std::vector<WeaponType> wep;


    /**
     * The spawner to use for this type of enemy.
     */
    SpawnType spawner;

    /**
     * Comparison operator for EnemyType.
     */
    bool operator==(const EnemyType& rhs) const
    {
        return this->id==rhs.id;
    }
};

namespace enemies
{
    extern EnemyType kobold;
    extern EnemyType rabbit;
    extern EnemyType wolf_companion;
    extern EnemyType ENEMY_LIST[3];
    extern int NUM_ENEMIES;
}

/**
 * Holds the information for the plant prototypes.
 * for now, it's really not much to look at.
 */
struct PlantType
{
    int id;
    std::vector<std::vector<Tile> > sprites;
    int harvest_tool;
    std::vector<MiscType> drops;
};

namespace plants
{
    extern PlantType tree;
    extern PlantType PLANT_LIST[1];
}


std::vector<std::vector<Tile> > construct_big_trees();

std::vector<std::vector<std::string> > construct_messages();

#endif
