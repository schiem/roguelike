/**
 *  @file DEFS.CPP
 *  @author Seth Yoder, Michael Yoder
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

#include <defs.h>

std::string STAT_NAMES[NUM_STATS] = {"Health", "Attack", "Endurance", "Str", "Dex", "Int"};


/****************************
 *   TERRAIN DEFS
 ***************************/
namespace tiledef {
    Tile EMPTY = {0, 0, 0, true, false, false, false};
    Tile OVERWORLD_DIRT = {250, 1, BROWN, true, false, false, false};
    Tile DIRT = {250, 2, BROWN, true, false, false, false};
    Tile DUNGEON_BORDER = {250, 3, DARK_RED, false, true, false, false};
    Tile ROOM_WALL = {219, 4, GRAY, false, false, true, false};
    Tile PATH = {250, 5, BROWN, true, false, false, false};
    Tile MAIN_CHAR = {1, 6, DARK_RED, true, true, false, false};
    Tile MAIN_CHAR2 = {1, 7, DARK_GREEN, true, true, false, false};
    Tile MAIN_CHAR3 = {1, 8, BROWN, true, true, false, false};
    Tile MAIN_CHAR4 = {1, 9, GRAY, true, true, false, false};
    Tile TREE = {84, 10, DARK_GREEN, false, false, true, false};
    Tile BLOCK_WALL = {219, 11, GRAY, false, false, true, false};
    Tile DOWN_STAIR = {31, 12, GRAY, true, false, false, false};
    Tile UP_STAIR = {30, 13, GRAY, true, false, false, false};
    Tile KOBOLD = {107, 14, RED, false, true, false, false};
    Tile KOBOLD_SPAWNER = {21, 15, GREEN, false, false, true, false};
    Tile WATER = {247, 16, BLUE, false, false, false, false};
    Tile LIGHT_WATER = {126, 17, BLUE, false, false, false, false};
    Tile SAND1 = {176, 18, TAN, true, false, false, false};
    Tile SAND2 = {178, 19, TAN, true, false, false, false};
    Tile BIG_TREE = {116, 20, DARK_GREEN, false, false, true, false};
    Tile GRASS_DIRT = {250, 21, DARKER_GREEN, true, false, false, false};
    Tile KOBOLD_CORPSE = {107, 22, GRAY, true, false, false, false};
    Tile BOOTS = {28, 23, BROWN, true, false, false, false};
    Tile RABBIT = {114, 24, BROWN, false, true, false, false};
    Tile RABBIT_CORPSE = {114, 25, GRAY, true, false, false, false};
    Tile SWORD = {47, 26, GRAY, true, false, false, false};
    Tile POTATO = {7, 27, BROWN, true, false, false, false};
    Tile PLANT = {6, 28, GREEN, true, false, false, false};
    Tile WOOD_WALL = {176, 29, BROWN, true, false, true, false};
    Tile WOOD_FLOOR = {47, 30, BROWN, true, false, false, false};
    Tile DOOR = {43, 31, BROWN, true, false, true, false};
    Tile BURROW = {15, 32, BROWN, true, false, false, false};
    Tile HUT_WALL = {35, 33, BROWN, false, false, false, false}; 


    Tile TILE_INDEX[TILE_TYPE_COUNT] = { //THIS MUST CORRESPOND TO TILE IDS
        EMPTY,          //ID 0
        OVERWORLD_DIRT, //ID 1
        DIRT,           //ID 2
        DUNGEON_BORDER, //etc
        ROOM_WALL,
        PATH,
        MAIN_CHAR,
        MAIN_CHAR2,
        MAIN_CHAR3,
        MAIN_CHAR4,
        TREE,
        BLOCK_WALL,
        DOWN_STAIR,
        UP_STAIR,
        KOBOLD,
        KOBOLD_SPAWNER,
        WATER,
        LIGHT_WATER,
        SAND1,
        SAND2,
        BIG_TREE,
        GRASS_DIRT,
        KOBOLD_CORPSE,
        BOOTS,
        RABBIT,
        RABBIT_CORPSE,
        SWORD,
        POTATO,
        PLANT,
        WOOD_WALL,
        WOOD_FLOOR,
        DOOR,
        BURROW,
        HUT_WALL
    };
}

namespace map_tile {
    MapTile MAP_DEFAULT = {240, DARK_GREEN, 0, true};
    MapTile MAP_WATER = {247, DARK_BLUE, 1, false};
    MapTile MAP_BEACH = {178, TAN, 2, false};
    MapTile MAP_FOREST = {116, DARK_GREEN, 3, true};
    MapTile CURSOR = {88, WHITE, 4, false};

    MapTile MAP_TILE_INDEX[5] = { //THIS MUST CORRESPOND TO MAP TILE IDS
        MAP_DEFAULT, //ID 0
        MAP_WATER,   //ID 1
        MAP_BEACH,   //ETC
        MAP_FOREST,
        CURSOR
    };
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


namespace equipment
{
    EquipType boots = {2.0, tiledef::BOOTS, "Boots", "A simple pair of leather boots.", 0, 3, 0, {0, 0, 0}, {.1, .1, 0}, false, 2};
}

namespace weapons
{
    WeaponType dagger = {2.0, tiledef::SWORD,  "Small Dagger", "An unfortunately pathetic dagger.", 0, 3, 2, 1, false, 1};
}

namespace consumables
{
    ConsumableType potato = {0.3, tiledef::PLANT, "Potato", "An uncooked potato. It might heal some health.  Or not, I don't know.", false, 0, HEALTH, rand() % 2, RESTORE, 1};
    ConsumableType CONSUMABLE_LIST[NUM_CONSUMABLES] = {potato};  
}

namespace misc
{
    MiscType kobold_corpse = {20, tiledef::KOBOLD_CORPSE, "Kobold Corpse", "A decaying corpse of a kobold. Yum!", false, 0, 10};
    MiscType rabbit_corpse = {3, tiledef::RABBIT_CORPSE, "Rabbit Corpse", "The corpse of a small woodland creature that met an untimely demise.", false, 0, 2};
    MiscType player_corpse = {120, tiledef::MAIN_CHAR, "Your Corpse", "You seem to have died.  Interesting...", false, 0, 30};
}

/****************************
 *   ENEMY DEFS
 ***************************/

namespace spawners
{
    SpawnType kobold = {true, 5, 20, 5, HUTS};
    SpawnType rabbit = {false, 5, 20, 20, BURROW};
}

//Enemy Equipment
EquipType kob_eq[] = {equipment::boots};
std::vector<EquipType> kob_eq_vec(&kob_eq[0], &kob_eq[0] + 1);
WeaponType kob_wep[] = {weapons::dagger};
std::vector<WeaponType> kob_wep_vec(&kob_wep[0], &kob_wep[0] + 1);


//Enemy Stats
//There has to be a better way to do this...
int kob_stats_arr[] = {10, 2, 10, 5, 5, 5};
std::vector<int> kob_stats(&kob_stats_arr[0], &kob_stats_arr[0] + NUM_STATS);

int rab_stats_arr[] = {2, 0, 10, 0, 0, 0};
std::vector<int> rab_stats(&rab_stats_arr[0], &rab_stats_arr[0] + NUM_STATS);

namespace enemies
{
    EnemyType kobold = {5, kob_stats, 0, 20, 5, 100, "Kobold", tiledef::KOBOLD, misc::kobold_corpse, kob_eq_vec, kob_wep_vec, spawners::kobold};
    EnemyType rabbit = {3, rab_stats, 1, 15, 25, (rand() % (20 + 50)), "Rabbit",  tiledef::RABBIT, misc::rabbit_corpse, std::vector<EquipType>(), std::vector<WeaponType>(), spawners::rabbit}; 
    EnemyType ENEMY_LIST[2] = {kobold, rabbit};
    int NUM_ENEMIES = 2; 
}
