#include "DungeonBuilder.h"

int main()
{
	DungeonBuilder dungeon(50, 40);
    dungeon.find_viable_starting_point();
    cout<<dungeon;
	return 0;
}
