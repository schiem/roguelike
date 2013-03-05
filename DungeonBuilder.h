#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#include <iostream>
#include <string>
#include <stdlib.h>
#include "int_point.h"
using namespace std;

class DungeonBuilder
{
    friend ostream& operator<<(ostream &out, const DungeonBuilder &D);
	private:
		int width;
		int height;
		char dungeon[MAX_WIDTH][MAX_HEIGHT];
		bool is_empty_space(IntPoint point);
		bool rolled_over(int given);
		int build_pblind_dungeon(int target, int deviation, int squareness);
		void print() const;

	public:
        IntPoint find_viable_starting_point();
		DungeonBuilder(int width, int height, int seed=time(NULL));
};

#endif
