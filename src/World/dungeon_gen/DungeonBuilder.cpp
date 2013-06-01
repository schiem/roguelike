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

    main_dungeon = Dungeon(width, height);
    srand(seed);
	bool initialized=false;
}

void DungeonBuilder::initialize(){
	initialized=true;
}

bool DungeonBuilder::is_initialized(){
	return initialized;
}


/*
 * POST: Will print the dungeon floor to stdout.
 */
/*
ostream& operator<<(ostream &out, const DungeonBuilder &D)
{
    D.print();
    return out;
}
*/

void DungeonBuilder::print(SDL_Surface* ascii, SDL_Surface* screen, int color) const
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			drawChr(i, j, main_dungeon.get_tile(i, j).char_count, ascii, screen, color);
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
    return ((main_dungeon.get_tile(point) == DIRT) or (main_dungeon.get_tile(point) == EMPTY));
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the given point is beyond the 
 * bounds of the dungeon.
 */
bool DungeonBuilder::point_is_beyond_bounds(IntPoint point) const
{
    if((point.row < 0) or (point.row >= main_dungeon.height))
    {
        return true;
    }

    else if((point.col < 0) or (point.col >= main_dungeon.width))
    {
        return true;
    }

    return false;
}

/* PRE: Will be given :IntPoint point: that lies on the wall of a room.
 * POST: Will determine which wall of the room the point lies on based
 *       on the surrounding tiles.
 */
int DungeonBuilder::determine_which_wall(IntPoint point) const
{
    int direction;

    if((main_dungeon.get_tile(point.row, point.col - 1) == WALL) or 
       (main_dungeon.get_tile(point.row, point.col + 1) == WALL))
    {
        if (main_dungeon.get_tile(point.row - 1, point.col) == DIRT)
        {
            direction = 2;
        }
        
        else if (main_dungeon.get_tile(point.row + 1, point.col) == DIRT)
        {
            direction = 0;
        }
    }

    else if ((main_dungeon.get_tile(point.row - 1, point.col) == WALL) or
        (main_dungeon.get_tile(point.row + 1, point.col) == WALL))
    {
        if (main_dungeon.get_tile(point.row, point.col - 1) == DIRT)
        {
            direction = 1;
        }
        
        else if (main_dungeon.get_tile(point.row, point.col + 1) == DIRT)
        {
            direction = 3;
        }
    }
    
    return direction;
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
    main_dungeon.set_tile(tl, WALL);
    cout<<"ROOM LEFT CORNER: "<<main_dungeon.get_tile(main_dungeon.rooms[0].tl).sprite;
    main_dungeon.set_tile(tl.row, br.col, WALL);
    main_dungeon.set_tile(br.row, tl.col, WALL);
    main_dungeon.set_tile(br, WALL);
    //draw top and bottom rows
    for(int i = tl.col + 1; i <= br.col - 1; i++)
    {
        main_dungeon.set_tile(tl.row, i, WALL);
        main_dungeon.set_tile(br.row, i, WALL);
    }
    //draw left and right WALLs
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        main_dungeon.set_tile(i, tl.col, WALL);
        main_dungeon.set_tile(i, br.col, WALL);
    }
    
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        for(int j = tl.col + 1; j <= br.col - 1; j++)
        {
            main_dungeon.set_tile(i, j, DIRT);
        }
    }

    return Room(tl, br);
}

/* PRE: Will be given :IntPoint the_point:
 * POST: Will find and return a viable room space, or a room with 
 *       tl.row, tl.col, br.row, br.col = '-1' if there is no viable space.
 */
Room DungeonBuilder::find_viable_room_space(IntPoint the_point) const
{
    return Room(IntPoint(-1, -1), IntPoint(-1, -1));
}

/* PRE:
 * POST: Will build a good starting room in the dungeon space
 */
void DungeonBuilder::build_start_room()
{
    int room_width = rand() % ROOM_WIDTH_DEV + 
                             (STD_ROOM_WIDTH - (int)(ROOM_WIDTH_DEV/2));
    int room_height = rand() % ROOM_WIDTH_DEV +
                             (STD_ROOM_HEIGHT - (int)(ROOM_HEIGHT_DEV/2));
    IntPoint starting_point = find_viable_starting_point(STD_ROOM_WIDTH, 
                                                        STD_ROOM_HEIGHT);
    IntPoint br = IntPoint(starting_point.row + (room_height + 1), 
                           starting_point.col + (room_width + 1));
    main_dungeon.rooms[num_rooms] = build_room(starting_point, br, 2);
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

/*
 * PRE: Will be given a Room object.
 * POST: Will return a random wall block that lies on the circumference of the given room.
 */
IntPoint DungeonBuilder::rand_wall_block(const Room &current_room)
{ 
    int height = (current_room.br.row - current_room.tl.row) - 1;
    int width = (current_room.br.col - current_room.tl.col) - 1;
    int path_from_side = rand() % (height + width) + 1;
    IntPoint point;
    if (path_from_side < height)
    {
        int a = rand() % 2;
        if (a == 0)
        {
            point.row = current_room.tl.row;
        }
        else
        {
            point.row = current_room.br.row;
        }
        point.col = rand() % width + current_room.tl.col + 1; 
    }
    else
    {
        int a = rand() % 2;
        if (a == 0)
        {
            point.col = current_room.tl.col;
        }
        else
        {
            point.col = current_room.br.col;
        }
        point.row = rand() % height + current_room.tl.row + 1;
    }

    return point;
}

/* PRE: Will be given :IntPoint this_point:, which is the current point, and 
 *      :int direction:, which refers to the proposed direction.
 * POST: Will return the next point given the proposed direction.
 */
IntPoint DungeonBuilder::get_next_point(IntPoint this_point, int direction) const
{
    IntPoint next_point = this_point;
    switch (direction)
    {
        case 0:
            next_point.row -= 1;
            break;
        case 1:
            next_point.col += 1;
            break;
        case 2:
            next_point.row += 1;
            break;
        case 3:
            next_point.col -= 1;
            break;
    }

    return next_point;
}   

/* PRE: Will be given :IntPoint start:, which refers to the wall block of the room at
 * which the path starts, and :int direction:, which refers to the starting direction
 * of the path according to this scheme:
 * 0 = up
 * 1 = right
 * 2 = down
 * 3 = left
 *
 * POST: Will attempt to build a path from the given Room and block. If this succeds,
 * will return the IntPoint of the end of the path. If it fails, it will return an 
 * IntPoint with row=-1 and col=-1.
 */
IntPoint DungeonBuilder::build_path(IntPoint start, int direction)
{
    int path_length = rand() % (MAX_PATH_LENGTH - MIN_PATH_LENGTH) + MIN_PATH_LENGTH;
    IntPoint current_point = start;
    int current_direction = direction;
    bool bad_direction;
    IntPoint potential_point;
    for(int i = 0; i < path_length; i++)
    {
        main_dungeon.set_tile(current_point, PATH);
        //For at least 2 or 3 blocks, just go straight. otherwise,
        //we may change direction.
        if (i > 3)
        {
            //Do we change direction?
            int dirchange = rand() % 100;
            if (dirchange < 15)
            {
                //Which direction do we change to?
                switch (dirchange % 2)
                {
                    case 0:
                        current_direction -= 1;
                        current_direction = (current_direction % 4);
                        break;
                    case 1:
                        current_direction += 1;
                        current_direction = (current_direction % 4);
                        break;
                }
            }
        }

        int tries = 0;
        do
        {
            bad_direction = false;
            potential_point = get_next_point(current_point, current_direction);
            if ((point_is_beyond_bounds(potential_point)) or (!is_empty_space(potential_point)))
            {
                //cout<<is_empty_space(potential_point)<<"..."<<main_dungeon.get_tile(potential_point).sprite<<endl;
                bad_direction = true;
                current_direction += 1;
            }
            tries++;
        } while((bad_direction) && tries < 4);
        
        current_point = potential_point;
        
    }
    return IntPoint(-1, -1);
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
int DungeonBuilder::build_pblind_dungeon(int target, 
                                         int deviation, int squareness)
{   
    build_start_room();
    //int target_rooms = rand() % deviation + 
    //                        (_target - (int)(deviation / 2));
    int current_room_num= 0;
    recursive_pblind_dungeon(target, deviation, squareness, current_room_num);
    //cout<<*this;
	return 0;
}

/* PRE: Will be given :int target:, :int deviation:, :int squareness:,
 * :int std_room_width:, :int room_width_deviation:, 
 * :int room_height_deviation: from the parent function :int build_pblind_dungeon():,
 * and :int current_room_num:, which is an index of the "rooms" array.
 *
 * POST: Will build the dungeon by finding 0 to 2 viable wall blocks in the given
 * room, building paths outward from those wall blocks, and (often) building rooms
 * at the end of those paths. Every time a new room is built, the function is called
 * again with that room's index passed as :int current_room_num:. 
 *
 */
void DungeonBuilder::recursive_pblind_dungeon(int target, int deviation,
                                             int squareness, int current_room_num)
{
    //declaring Room as pointer to point to different array indices.
    Room current_room = main_dungeon.rooms[current_room_num];
    IntPoint point = rand_wall_block(current_room);
    //build_path(point, determine_which_wall(point));
    build_path(point, 2);
    //main_dungeon.set_tile(point, PATH);
}
