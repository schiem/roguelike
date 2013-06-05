#include "terrain_defs.h"

namespace tiledef {
    Tile DIRT = {250, 1, '.', true};
    Tile WALL = {35, 2, '#', false};
    Tile EMPTY = {0, 3, ' ', true};
    //static const Tile PATH = {88, 'X', true};
    //static const Tile PATH = {111, 'o', true};
    Tile PATH = {240, 4, ' ', true};
    Tile MAIN_CHAR = {1, 5, ' ', true}; 
}
