#include <Chunk.h>
#include <iostream>
using namespace std;

Chunk::Chunk(int _x, int _y, int _width, int _height)
{
	width = _width;
	height= _height;
	depth = rand() % 6;
	//dungeon = new Dungeon[depth];
    dungeon_floors = vector<Dungeon>(depth);//, Dungeon(width, height));
	x = _x;
	y = _y;
	DungeonBuilder db = DungeonBuilder(width, height); //doesn't this take 3 parameters?
	//fill dungeon array with dungeons          //The last parameter is a seed with a default of time(NULL).
	for (int i=0; i <= depth; i++){
		db.build_pblind_dungeon(5, 5, 5);
        Dungeon* temp_d = db.get_dungeon();
		dungeon_floors[i] = *temp_d;
	}
	//generate the overworld
	overworld = Overworld(width, height);
}

Chunk::Chunk(){
    /*
	depth = rand() % 6;
	//dungeon = new Dungeon[depth];
    dungeon_floors = vector<Dungeon>(depth);
	x = 0;
	y = 0;
	height = 10;
	width = 10;
	DungeonBuilder db = DungeonBuilder(width, height); 
	for (int i=0; i <= depth; i++){
		db.build_pblind_dungeon(5, 5, 5);
		dungeon_floors[i] = db.get_dungeon();
	}
	overworld = Overworld(width, height);
    */
}

/*
Chunk::~Chunk(){
	delete []dungeon;
}
*/

const std::vector<std::vector<Tile> >& Chunk::get_floor(int depth){
	if (depth == 0){
		return overworld.get_ground();
	}
	else{
		return dungeon_floors[depth].get_dungeon();
	}
}

Tile Chunk::get_tile(int depth, int col, int row) const
{
	if (depth == 0){
		return overworld.get_tile(col, row);
	}
	else{
		return dungeon_floors[depth].get_tile(col, row);
	}
}
