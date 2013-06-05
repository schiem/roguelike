#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H

struct Tile{
	int char_count;
    int tile_id;
	char sprite;
	bool can_be_moved_through;
	bool operator==(const Tile& rhs) const{
		return this->sprite == rhs.sprite;
		}
	};

static const Tile DIRT = {250, 1, '.', true};
static const Tile WALL = {35, 2, '#', false};
static const Tile EMPTY = {0, 3, ' ', true};
//static const Tile PATH = {88, 'X', true};
//static const Tile PATH = {111, 'o', true};
static const Tile PATH = {240, 4, ' ', true};
static const Tile MAIN_CHAR = {1, 5, ' ', true}; 

#endif

