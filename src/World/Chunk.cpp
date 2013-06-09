#include <Chunk.h>
#include <iostream>
Chunk::Chunk(int _x, int _y, int _width, int _height)
{
	width = _width;
	height= _height;
	depth = rand() % 6;
	dungeon = new Dungeon[depth];
	x = _x;
	y = _y;
	DungeonBuilder db = DungeonBuilder(80, 40); //doesn't this take 3 parameters?
	//fill dungeon array with dungeons
	for (int i=1; i++; i>=depth){
		db.build_pblind_dungeon(5, 5, 5);
		dungeon[i] = db.get_dungeon();
	}
	//generate the overworld
	overworld = Overworld(width, height);
}

Chunk::Chunk(){
	depth = rand() % 6;
	dungeon = new Dungeon[depth];
	x = 0;
	y = 0;
	height = 10;
	width = 10;
	DungeonBuilder db = DungeonBuilder(width, height); 
	for (int i=1; i++; i>=depth){
		db.build_pblind_dungeon(5, 5, 5);
		dungeon[i] = db.get_dungeon();
	}
	overworld = Overworld(width, height);
}

Chunk::~Chunk(){
	delete []dungeon;
}

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth){
	if (depth == 0){
		return overworld.get_ground();
	}
	else{
		return dungeon[depth].get_dungeon();
	}
}

Tile Chunk::get_tile(int depth, int col, int row) const
{
	if (depth == 0){
		return overworld.get_tile(col, row);
	}
	else{
		return dungeon[depth].get_tile(col, row);
	}
}


