#include "DungeonBuilder.h"

/* PRE:  Will be given the desired width and height of the
 * dungeon floor.
 * POST: Will populate the :char dungeon[][]: array with dummy
 * values.
 */
DungeonBuilder::DungeonBuilder(int _width, int _height)
{
	width = _width;
	height = _height;

	if(width > MAX_WIDTH)
	{
		width = MAX_WIDTH;
	}
	if(height > MAX_HEIGHT)
	{
		height = MAX_HEIGHT;
	}

	cout<<"width:"<<_width<<"  height:"<<_height<<endl;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			dungeon[i][j] = '.';
		}
	}
}

/*
 * POST: Will print the dungeon floor to stdout.
 */
void DungeonBuilder::print()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cout<<dungeon[i][j]<<" ";
		}
		cout<<endl;
	}
}

/* PRE: Will be given :int given:, a number under 100.
 *
 * POST: Will perform a random number check between 1 and 100.
 * Will return true if result <= given; will return false if 
 * result > given.
 */
bool DungeonBuilder::rolled_over(int given)
{
	int generated = rand() % 100;
	if(generated <= given)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the point is an empty
 * space.
 */
bool DungeonBuilder::is_empty_space(IntPoint point)
{
	if(dungeon[point.x][point.y] == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* PRE: Will be given :int squareness:, a number from 0 to 100,
 * which determines how square the room is, and 
 * :IntPoint starting_point:, which specifies the top left 
 * corner of the zone of generation.
 *
 * POST: Will build a single opening, starting at the top left 
 * corner. With maximum "squareness", the opening will be
 * completely square.
 */
bool DungeonBuilder::generate_opening(int squareness, IntPoint starting_point)
{
	return true;
}
/* PRE: Will be given :int target: to specify a general target
 * number of openings in the dungeon floor, :int deviation: to
 * specify the maximum desired deviation from this target, and
 * :int squareness:, a number from 0 to 100, which determines 
 * how square the rooms are.
 *
 * POST: Will build openings in the dungeon floor and return
 * the number of openings that were built.
 */
int DungeonBuilder::build_openings(int target, int deviation, int squareness)
{
	int openings = 0;
	IntPoint starting_point;
	starting_point.x = rand() % (width - 5);
	starting_point.y = rand() % (height - 5);
	generate_opening(squareness, starting_point);
	return openings;
}

/* PRE: Will be given :int squareness: to specify how square
 * the built hallways will be.
 *
 * POST: Will link openings in the dungeon floor together with
 * "hallways" or passages.
 */
int DungeonBuilder::build_hallways(int squareness)
{
	int links = 0;
	return links;
}
