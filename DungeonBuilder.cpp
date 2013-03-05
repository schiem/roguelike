#include "DungeonBuilder.h"

/* PRE:  Will be given the desired width and height of the
 * dungeon floor.
 * POST: Will populate the :char dungeon[][]: array with dummy
 * values.
 */
DungeonBuilder::DungeonBuilder(int _width, int _height, int seed)
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
    srand(seed);
    cout<<seed;
}

/*
 * POST: Will print the dungeon floor to stdout.
 */
void DungeonBuilder::print() const
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

ostream& operator<<(ostream &out, const DungeonBuilder &D)
{
    D.print();
    return out;
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

IntPoint DungeonBuilder::find_viable_starting_point()
{
    int good_row = rand() % (int)(height/2) + (int)(height/4);
    int good_col = rand() % (int)(width/2) + (int)(width/4);
    dungeon[good_row][good_col] = '@';
}

/* PRE: Will be given :int target: to specify a general target
 * number of openings in the dungeon floor, :int deviation: to
 * specify the maximum desired deviation from this target, and
 * :int squareness:, a number from 0 to 100, which determines 
 * how square the rooms are.
 *
 * POST: Will build a procedurally-blind dungeon in the dungeon
 * floor (in which a room is built near the center, and rooms and
 * hallways crawl off of that room.
 */
int DungeonBuilder::build_pblind_dungeon(int target_openings, 
                                         int deviation, int squareness)
{
	int openings = 0;
	return openings;
}
