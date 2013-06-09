#ifndef _CHUNK_H
#define _CHUNK_H
#include <vector>
#include <stdlib.h>
#include <DungeonBuilder.h>
#include <Dungeon.h>
#include <Overworld.h>
#include <terrain_defs.h>
using namespace tiledef;
class Chunk{
	typedef std::vector<std::vector<Tile> > TileMatrix;
	private:
		int x;
		int y;
		int depth;
		Dungeon* dungeon;
		Overworld overworld;
	public:
		Chunk(int, int, int, int);
		int height;
		int width;
		Chunk();
		~Chunk();
		const std::vector<std::vector<Tile> >& get_floor(int);
		Tile get_tile(int, int, int) const;

};

#endif

					
