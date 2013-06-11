#include "terrain_defs.h"6

namespace tiledef {
    Tile DIRT = {250, 1, '.', true, 16777215};
    Tile WALL = {35, 2, '#', false, 16777215};
    Tile EMPTY = {0, 3, ' ', true, 0};
    //static const Tile PATH = {88, 'X', true};
    //static const Tile PATH = {111, 'o', true};
    Tile PATH = {240, 4, ' ', true, 16777215};
    Tile MAIN_CHAR = {1, 5, ' ', true, 16711680}; 
	Tile TREE = {84, 6, 'T', false, 65280};
}
