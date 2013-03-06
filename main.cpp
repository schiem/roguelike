#include "DungeonBuilder.h"

int main()
{
	DungeonBuilder dungeon(50, 40);
    dungeon.build_pblind_dungeon(1, 1, 1);

	return 0;
}
