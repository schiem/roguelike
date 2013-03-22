#ifndef WORLD_H
#define WORLD_H

using namespace std;

struct Terrain{
	char sprite;
	bool can_move_on;
};

class World{
    private:
        Terrain dungeon[100][100];
    public:
        void create_dungeon(Terrain new_dungeon[100][100]);
        void display_dungeon();
};
#endif
