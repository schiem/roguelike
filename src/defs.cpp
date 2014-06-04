/**
 *  DEFS.CPP
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

#include <defs.h>

/****************************
 *   TERRAIN DEFS
 ***************************/
namespace tiledef {
    Tile OVERWORLD_DIRT = {250, 1, BROWN, true, false, false, false};
    Tile DIRT = {250, 2, BROWN, true, false, false, false};
    Tile DUNGEON_BORDER = {250, 3, DARK_RED, false, true, false, false};
    Tile ROOM_WALL = {219, 4, GRAY, false, false, true, false};
    Tile EMPTY = {0, 5, 0, true, false, false, false};
    Tile PATH = {250, 6, BROWN, true, false, false, false};
    Tile MAIN_CHAR = {1, 7, DARK_RED, true, true, false, false};
    Tile MAIN_CHAR2 = {1, 8, DARK_GREEN, true, true, false, false};
    Tile MAIN_CHAR3 = {1, 9, BROWN, true, true, false, false};
    Tile MAIN_CHAR4 = {1, 10, GRAY, true, true, false, false};
    Tile TREE = {84, 11, DARK_GREEN, false, false, true, false};
    Tile BLOCK_WALL = {219, 12, GRAY, false, false, true, false};
    Tile DOWN_STAIR = {31, 13, GRAY, true, false, false, false};
    Tile UP_STAIR = {30, 14, GRAY, true, false, false, false};
    Tile KOBOLD = {107, 15, RED, false, true, false, false};
    Tile KOBOLD_SPAWNER = {21, 16, GREEN, false, false, true, false};
    Tile WATER = {247, 17, BLUE, false, false, false, false};
    Tile LIGHT_WATER = {126, 18, BLUE, false, false, false, false};
    Tile SAND1 = {250, 19, TAN, true, false, false, false};
    Tile SAND2 = {250, 20, DARK_TAN, true, false, false, false};
    Tile BIG_TREE = {116, 21, DARK_GREEN, false, false, true, false};
    Tile GRASS_DIRT = {250, 22, DARKER_GREEN, true, false, false, false};
    Tile KOBOLD_CORPSE = {107, 23, GRAY, true, false, false, false};
    Tile BOOTS = {28, 24, BROWN, true, false, false, false};
    Tile RABBIT = {114, 25, BROWN, false, true, false, false};
    Tile RABBIT_CORPSE = {114, 26, GRAY, true, false, false, false};
    Tile SWORD = {47, 26, GRAY, true, false, false, false};
}

namespace map_tile {
    MapTile MAP_DEFAULT = {240, DARK_GREEN, 1, true};
    MapTile MAP_WATER = {247, DARK_BLUE, 2, false};
    MapTile MAP_BEACH = {178, TAN, 3, false};
    MapTile MAP_FOREST = {116, DARK_GREEN, 4, true};
    MapTile CURSOR = {88, WHITE, 5, false};
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
    EquipType boots = {3, tiledef::BOOTS, "Boots", 0, 3, 0, 5, false};
}

namespace weapons
{
    WeaponType dagger = {1, tiledef::SWORD, "Small Dagger", 0, 3, 2, 1, false};
}

/****************************
 *   ENEMY DEFS
 ***************************/

EquipType kob_eq[] = {equipment::boots};
std::vector<EquipType> kob_eq_vec(&kob_eq[0], &kob_eq[0] + 1);
WeaponType kob_wep[] = {weapons::dagger};
std::vector<WeaponType> kob_wep_vec(&kob_wep[0], &kob_wep[0] + 1);



namespace enemies
{
    EnemyType kobold = {5, 20, 2, 0, 1, 20, 100, "Kobold", tiledef::KOBOLD, tiledef::KOBOLD_CORPSE, kob_eq_vec, kob_wep_vec};
    EnemyType rabbit = {3, 10, 0, 0, 2, 15, (rand() % (20 + 50)), "Rabbit",  tiledef::RABBIT, tiledef::RABBIT_CORPSE, std::vector<EquipType>(), std::vector<WeaponType>()}; 
}
