#define MAX_WIDTH 500 
#define MAX_HEIGHT 500
#include <iostream>
#include <string>
#include <stdlib.h>
#include "int_point.h"
using namespace std;

class DungeonBuilder
{
	private:
		int width;
		int height;
		char dungeon[MAX_WIDTH][MAX_HEIGHT];
		bool is_empty_space(IntPoint point);
		bool rolled_over(int given);
		int build_openings(int target, int deviation, int squareness);
		int build_hallways(int squareness);
		bool generate_opening(int squareness, IntPoint starting_point);

	public:
		DungeonBuilder(int width, int height);
		void print();
};
