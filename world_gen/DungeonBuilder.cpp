#include <DungeonBuilder.h>

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
bool DungeonBuilder::rolled_over(int given) const
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
bool DungeonBuilder::is_empty_space(IntPoint point) const
{
    return (dungeon[point.row][point.col] == '.');
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the given point is beyond the 
 * bounds of the dungeon.
 */
bool DungeonBuilder::point_is_beyond_bounds(IntPoint point) const
{
    if((point.row < 0) or (point.row >= height))
    {
        return true;
    }

    else if((point.col < 0) or (point.col >= width))
    {
        return true;
    }

    return false;
}

/* PRE:
 * POST: Will find a good starting point for a procedurally-blind dungeon
 */
IntPoint DungeonBuilder::find_viable_starting_point(int std_width, int std_height) const
{
    int good_row = rand() % (int)(height/2) + (int)(height/4) - (int)(std_height / 2);
    int good_col = rand() % (int)(width/2) + (int)(width/4) - (int)(std_width / 2);
    return IntPoint(good_row, good_col);
}

/* PRE: Will be given :IntPoint tl:, which represents the top-left corner,
 * :IntPoint br:, which represents the bottom-right corner, and
 * :int squareness:, which denotes how square the rooms will be.
 * 
 * POST: Will draw a room on the dungeon array with the given parameters.
 */
Room DungeonBuilder::build_room(IntPoint tl, IntPoint br, int squareness)
{
    //draw four corners
    dungeon[tl.row][tl.col] = '+';
    dungeon[tl.row][br.col] = '+';
    dungeon[br.row][tl.col] = '+';
    dungeon[br.row][br.col] = '+';
    //draw top and bottom rows
    for(int i = tl.col + 1; i <= br.col - 1; i++)
    {
        dungeon[tl.row][i] = '=';
        dungeon[br.row][i] = '=';
    }
    //draw left and right walls
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        dungeon[i][tl.col] = '|';
        dungeon[i][br.col] = '|';
    }

    return Room(tl, br);
}

/* PRE:
 * POST: Will build a good starting room in the dungeon space
 */
void DungeonBuilder::build_start_room(int std_room_width, int std_room_height,
                                int room_width_deviation, int room_height_deviation)
{
    int room_width = rand() % room_width_deviation + 
                             (std_room_width - (int)(room_width_deviation/2));
    int room_height = rand() % room_height_deviation +
                             (std_room_height - (int)(room_height_deviation/2));
    IntPoint starting_point = find_viable_starting_point(std_room_width, 
                                                        std_room_height);
    IntPoint br = IntPoint(starting_point.row + (room_height + 1), 
                           starting_point.col + (room_width + 1));
    rooms[num_rooms] = build_room(starting_point, br, 2);
    num_rooms += 1;
}

/* PRE: Will be given a Room object.
 * POST: Will return the original number of wall spaces in the room 
 * (corners are not included).
 */
int DungeonBuilder::get_wall_count(const Room &R) const
{
    return (R.br.row - R.tl.row - 1) * 2 + (R.br.col - R.tl.col - 1) * 2;
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
int DungeonBuilder::build_pblind_dungeon(int _target, 
                                         int deviation, int squareness)
{   
    int std_room_width = 10;
    int std_room_height = 10;
    int room_width_deviation = 12;
    int room_height_deviation = 8;
	int openings = 0;
    build_start_room(std_room_width, std_room_height, room_width_deviation, room_height_deviation);    
    //int target_rooms = rand() % deviation + 
    //                        (_target - (int)(deviation / 2));
    bool finished = false;
    int current_room_num= 0;

    //Do the actual dungeon crawling. COMPLETELY UNIFINISHED
    while(!finished)
    {
        //declaring Room as pointer to point to different array indices.
        Room * current_room;
        current_room = &rooms[current_room_num]; 
        cout<<get_wall_count(*current_room)<<endl;
        cout<<*this<<endl;
        finished = true;
    }
    
	return openings;
}
