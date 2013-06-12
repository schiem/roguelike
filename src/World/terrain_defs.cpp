#include "terrain_defs.h"

namespace tiledef {
    Tile DIRT = {250, 1, '.', true, BROWN};
    Tile WALL = {35, 2, '#', false, GRAY};
    Tile EMPTY = {0, 3, ' ', true, 0};
    //static const Tile PATH = {88, 'X', true};
    //static const Tile PATH = {111, 'o', true};
    Tile PATH = {240, 4, ' ', true, GRAY};
    Tile MAIN_CHAR = {1, 5, ' ', true, DARK_RED}; 
	Tile TREE = {84, 6, 'T', false, DARK_GREEN};
}
