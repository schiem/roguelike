#include <Chunk.h>
using namespace std;

Chunk::Chunk(int _x, int _y, int _width, int _height)
{
	width = _width;
	height= _height;
	srand(time(NULL));
	depth = rand() % 6;
	//dungeon = new Dungeon[depth];
    dungeon_floors = vector<Dungeon>(depth, Dungeon(width, height));
	x = _x;
	y = _y;
    Dungeon* temp_d;
	DungeonBuilder db(width, height); //doesn't this take 3 parameters? ---The last parameter is a seed that defaults to time(NULL).
	//fill dungeon array with dungeons  

    //Currently, building a new dungeon just does it right on top of the old dungeon. Which actually
    //produces some pretty cool-looking dungeons. But anyway, we need to actually refresh DungeonBuilder's
    //dungeon... Possibly by reconstructing db on every new loop? So then, the problem with that will be that
    //sometimes there will be no way for the character to traverse between floors (if no rooms overlap). A solution
    //there is to pick the last room in the "rooms" vector as the "destination" room, which will have stairs to
    //the lower level. The lower level of the dungeon would then be built off of that point. This probably means
    //making some changes to build_pblind_dungeon().
	cout<<"depth:"<<depth<<endl;
	for (int i=0; i < depth; i++){
		cout<<"floor "<<i<<" reporting"<<endl;
		db.build_pblind_dungeon(5, 5, 5);
        temp_d = db.get_dungeon();
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
	if (depth == -1){
		return overworld.get_ground();
	}
	else{
		return dungeon_floors[depth].get_dungeon();
	}
}

Tile Chunk::get_tile(int depth, int col, int row) const
{
	if (depth ==-1){
		return overworld.get_tile(col, row);
	}
	else{
		return dungeon_floors[depth].get_tile(col, row);
	}
}

int Chunk::get_depth() const
{
	return depth;
}
