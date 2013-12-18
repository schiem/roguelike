#include "terrain_defs.h"

namespace tiledef {
    Tile OVERWORLD_DIRT = {250, 1, true, true, BROWN};
    Tile DIRT = {250, 1, true, false, BROWN};
    Tile WALL = {35, 2, false, false, GRAY};
    Tile EMPTY = {0, 3, true, false, 0};
    Tile PATH = {240, 4, true, false, GRAY};
    Tile MAIN_CHAR = {1, 5, true, true, DARK_RED}; 
    Tile MAIN_CHAR2 = {1, 5, true, true, DARK_GREEN};
    Tile MAIN_CHAR3 = {1, 5, true, true, BROWN};
    Tile MAIN_CHAR4 = {1, 5, true, true, GRAY};
    Tile TREE = {84, 6, false, true, DARK_GREEN};
}
