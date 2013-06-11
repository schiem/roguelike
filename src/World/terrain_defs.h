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
	int color;
	};

namespace tiledef {

    extern Tile DIRT;
    extern Tile WALL;
    extern Tile EMPTY;
    //static const Tile PATH = {88, 'X', true};
    //static const Tile PATH = {111, 'o', true};
    extern Tile PATH;
    extern Tile MAIN_CHAR; 
	extern Tile TREE;
}

#endif
