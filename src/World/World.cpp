#include <World.h>

Terrain dirt = {' ', true};
Terrain wall = {'#', false};


World::create_dungeon(Terrain new_dungeon[100][100]){
	dungeon = new_dungeon;
}

World::display_dungeon(){
	for(int x=0; x<100;x++){
		for(int y=0; y<100; y++){
			mvaddch(y, x, dungeon[x][y]);
		}
	}
}


}

