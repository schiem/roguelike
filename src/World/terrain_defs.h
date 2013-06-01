#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H

struct Tile{
	int char_count;
	char sprite;
	bool can_be_moved_through;
	bool operator==(const Tile& rhs) const{
		return this->sprite == rhs.sprite;
		}
	};

static const Tile DIRT = {250, '.', true};
static const Tile WALL = {35, '#', false};
static const Tile EMPTY = {0, ' ', true};
static const Tile PATH = {88, 'X', true};

#endif

