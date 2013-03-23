#ifndef TERRAIN_DEFS_H
#define TERRAIN_DEFS_H

struct Tile{
	char sprite;
	bool can_be_moved_through;
	bool operator==(const Tile& rhs) const{
		return this->sprite == rhs.sprite;
		}
	};

static const Tile DIRT = {'.', true};
static const Tile WALL = {'#', false};
static const Tile EMPTY = {' ', false};
static const Tile PATH = {'X', true};

#endif

