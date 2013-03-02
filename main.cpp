#include "DungeonBuilder.h"

int main()
{
	DungeonBuilder dungeon(50, 40);
	dungeon.print();
	cout<<(rand() % 100) / 100.0<<endl;
	return 0;
}
