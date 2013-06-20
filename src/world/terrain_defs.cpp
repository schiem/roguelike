#include "terrain_defs.h"

namespace tiledef {
    Tile DIRT = {250, 1, '.', true, BROWN};
    Tile WALL = {35, 2, '#', false, GRAY};
    Tile EMPTY = {0, 3, ' ', true, 0};
    Tile PATH = {240, 4, ' ', true, GRAY};
    Tile MAIN_CHAR = {1, 5, ' ', true, DARK_RED}; 
    Tile MAIN_CHAR2 = {1, 5, ' ', true, DARK_GREEN};
    Tile MAIN_CHAR3 = {1, 5, ' ', true, BROWN};
    Tile MAIN_CHAR4 = {1, 5, ' ', true, GRAY};
    Tile TREE = {84, 6, 'T', false, DARK_GREEN};
}
