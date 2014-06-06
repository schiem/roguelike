/**
 *  TERRAIN_DEFS.CPP
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

#include <terrain_defs.h>

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

    Tile TILE_INDEX[24] = { //THIS MUST CORRESPOND TO TILE IDS
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
        BOOTS
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

