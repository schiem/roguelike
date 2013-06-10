#include "DungeonBuilder.h"

using namespace tiledef;

DungeonBuilder::DungeonBuilder()
{
    width = 80;
    height = 40;
    main_dungeon = Dungeon(width, height);
    num_rooms = 0;
}

DungeonBuilder::DungeonBuilder(const DungeonBuilder& db)
{
    width = db.width;
    height = db.height;
    num_rooms = db.num_rooms;
    main_dungeon = db.main_dungeon;
}

/* PRE:  Will be given the desired width and height of the
 * dungeon floor.
 * POST: Will populate the :char dungeon[][]: array with dummy
 * values.
 */
DungeonBuilder::DungeonBuilder(int _width, int _height, int seed)
{
	width = _width;
	height = _height;
    num_rooms = 0;

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
}

void DungeonBuilder::print(SDL_Surface* ascii, SDL_Surface* screen, int color) const
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			drawChr(i, j, main_dungeon.get_tile(i, j).char_count, ascii, screen, color);
		}
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
    return ((main_dungeon.get_tile(point) == EMPTY) || (main_dungeon.get_tile(point) == PATH));
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

/* PRE: Will be given :Room &r:
 * POST: Returns a string of 4 1s or 0s:
 *       Order: Top, right, bottom, left. A 1 means something collided,
 *       and a 0 means nothing collided.
 */
string DungeonBuilder::edges_collide_with_something(Room& r) const
{
    string bin_string = "0000";
    for(int row = r.tl.row; row <= r.br.row; row++)
    {
        if (point_is_beyond_bounds(IntPoint(row, r.tl.col))) {
            bin_string[3] = '1';
        } else if (main_dungeon.get_tile(row, r.tl.col).can_be_moved_through == false) {
            bin_string[3] = '1';
        }

        if (point_is_beyond_bounds(IntPoint(row, r.tl.col))) {
            bin_string[1] = '1';
        } else if (main_dungeon.get_tile(row, r.br.col).can_be_moved_through == false) {
            bin_string[1] = '1';
        }
    }

    for(int col = r.tl.col; col <= r.br.col; col++)
    {
        if (point_is_beyond_bounds(IntPoint(r.tl.row, col))) {
            bin_string[0] = '1';
        } else if (main_dungeon.get_tile(r.tl.row, col).can_be_moved_through == false) {
            bin_string[0] = '1';
        }

        if (point_is_beyond_bounds(IntPoint(r.br.row, col))) {
            bin_string[2] = '1';
        } else if (main_dungeon.get_tile(r.br.row, col).can_be_moved_through == false) {
            bin_string[2] = '1';
        }
    }
    return bin_string;
}

/* PRE: Will be given :IntPoint point: that lies on the wall of a room.
 * POST: Will determine which wall of the room the point lies on based
 *       on the surrounding tiles.
 */
int DungeonBuilder::determine_which_wall(IntPoint point) const
{
    int direction;

    if(point.col == 0) {
        return 3;
    } 

    else if (point.row == 0) {
        return 0;
    } 

    else if((main_dungeon.get_tile(point.row, point.col - 1) == WALL) or
       (main_dungeon.get_tile(point.row, point.col + 1) == WALL)) {

        if (main_dungeon.get_tile(point.row - 1, point.col) == DIRT) {
            direction = 2;
        }
        
        else if (main_dungeon.get_tile(point.row + 1, point.col) == DIRT) {
            direction = 0;
        }
    }

    else if ((main_dungeon.get_tile(point.row - 1, point.col) == WALL) or
        (main_dungeon.get_tile(point.row + 1, point.col) == WALL)) {

        if (main_dungeon.get_tile(point.row, point.col - 1) == DIRT) {
            direction = 1;
        }
        
        else if (main_dungeon.get_tile(point.row, point.col + 1) == DIRT) {
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

/* PRE: Will be given :int a: and :int b:, representing a row and column.
 * POST: If that row and column is not currently a PATH tile, set it to a 
 *       WALL tile.
 */
void DungeonBuilder::set_wall_if_not_path(int a, int b) 
{
    if(main_dungeon.get_tile(a, b).tile_id != 4)
    {
        main_dungeon.set_tile(a, b, WALL);
    }
}
/* PRE: Will be given :IntPoint tl:, which represents the top-left corner,
 * :IntPoint br:, which represents the bottom-right corner, and
 * :int squareness:, which denotes how square the rooms will be.
 * 
 * POST: Will draw a room on the dungeon array with the given parameters.
 */
Room DungeonBuilder::build_room(IntPoint tl, IntPoint br, int squareness)
{
    set_wall_if_not_path(tl.row, tl.col);
    set_wall_if_not_path(tl.row, br.col);
    set_wall_if_not_path(br.row, tl.col);
    set_wall_if_not_path(br.row, br.col);
        
    //draw top and bottom rows
    for(int i = tl.col + 1; i <= br.col - 1; i++)
    {
        set_wall_if_not_path(tl.row, i);
        set_wall_if_not_path(br.row, i);
    }
    //draw left and right WALLs
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        set_wall_if_not_path(i, tl.col);
        set_wall_if_not_path(i, br.col);
    }
    
    for(int i = tl.row + 1; i <= br.row - 1; i++)
    {
        for(int j = tl.col + 1; j <= br.col - 1; j++)
        {
            main_dungeon.set_tile(i, j, DIRT);
        }
    }
    num_rooms++;
    main_dungeon.rooms[num_rooms] = Room(tl, br);
    
    return Room(tl, br);
}

/* PRE: Will be given :IntPoint the_point:
 * POST: Will find and return a viable room space, or a room with 
 *       tl.row, tl.col, br.row, br.col = '-1' if there is no viable space.
 */
Room DungeonBuilder::find_viable_room_space(IntPoint the_point) const
{
    /*
     *  pseudocode for this method:
     *
     *  declare min room width and min room height;
     *  declare and define test_room based on this width and height;
     *  declare upper_bound, lower_bound, left_bound, right_bound = 1
     *
     *  if test_room collides with something solid:
     *      return something nullish
     *
     *  while (room width < max) and (room height < max) AND
     *      (upper_bound + lower_bound + left_bound + right_bound > 0):
     *
     *      move all of test_room's points out: (subroutine probably)
     *          subtract upper_bound from row value of both upper points;
     *          subtract left_bound from col value of both left side points;
     *          add lower_bound to row value of both lower points;
     *          add right_bound to col value of both right side points;
     *
     *      "scan" across those newly created edges;
     *
     *      if, during the scan, we ran into a solid block:
     *          which side did it occur on?
     *          Set the *_bound to 0 for that side;
     *          move the points on that side one step toward the room center;
     *
     */

    int min_room_width = STD_ROOM_WIDTH - (ROOM_WIDTH_DEV / 2);
    int min_room_height = STD_ROOM_HEIGHT - (ROOM_HEIGHT_DEV / 2);

    //create a room centered around the end of the path. floor() and ceil() derived from math.h.
    Room test_room = Room(IntPoint((the_point.row - (int) ceil(min_room_height / 2.0)), 
                           (the_point.col - (int) ceil(min_room_width / 2.0) + 1)),
                  IntPoint((the_point.row + (int) floor(min_room_height / 2.0) + 1),
                           (the_point.col + (int) floor(min_room_width / 2.0))));

    //Check if the room is out-of-bounds; if so, return a null room
    /*
    if((point_is_beyond_bounds(test_room.tl)) || (point_is_beyond_bounds(test_room.br)))
    {
        return Room(IntPoint(-1, -1), IntPoint(-1, -1));
    }
    */

    //Check the room's walls to see if they clip with anything else.
    if (edges_collide_with_something(test_room) != "0000")
    {
        return Room(IntPoint(-1, -1), IntPoint(-1, -1));
    }

    int upper_bound = 1;
    int lower_bound = 1;
    int left_bound = 1;
    int right_bound = 1;
    
    //Here, we should have a test_room that doesn't collide with things.
    int max_room_width = STD_ROOM_WIDTH + (ROOM_WIDTH_DEV / 2);
    int max_room_height = STD_ROOM_HEIGHT + (ROOM_HEIGHT_DEV / 2);

    //until each rectangle edge is unable to grow any more:
    while ((test_room.br.col - test_room.tl.col < max_room_width) &&
           (test_room.br.row - test_room.tl.row < max_room_height) &&
           (upper_bound + lower_bound + left_bound + right_bound > 0))
    { 
        //move the edges out
        test_room.tl.row -= upper_bound;
        test_room.tl.col -= left_bound;
        test_room.br.row += lower_bound;
        test_room.br.col += right_bound;

        //basically, if we ran into a solid block, set *_bound to 0 for that edge,
        //then move that edge one step toward the room center...
        string collision_bin_str = edges_collide_with_something(test_room);
        if(collision_bin_str[0] == '1'){
            upper_bound = 0;
            test_room.tl.row += 1;
        }
        if(collision_bin_str[1] == '1'){
            right_bound = 0;
            test_room.br.col -= 1;
        }
        if(collision_bin_str[2] == '1'){
            lower_bound = 0;
            test_room.br.row -= 1;
        }
        if(collision_bin_str[3] == '1'){
            left_bound = 0;
            test_room.tl.col += 1;
        }
    }
    //Here, test_room should be a viable area in which to stick a real room.

    /*
     *  problems:
     *      -shouldn't just use the largest room possible, that would be dumb. Instead, use that space
     *          to create a random room. But how to ensure that the path intersects this room?
     *
     *      solution to that issue:
     *
     *          -start with a left column that is between the left rectangle wall and (the lesser value of:
     *              (the right wall column - MIN_ROOM_WIDTH), the path column)
     *
     *          -get a right column somewhere between (the larger value of: (left room column + MIN_ROOM_WIDTH),
     *              the path column) and the right wall
     *            
     *          -get a top row somewhere between the top wall and (the lesser value of:
     *              (the bottom wall row - MIN_ROOM_HEIGHT),the path row)
     *
     *          -get a bottom row somewhere between (the larger value of: (top room row + MIN_ROOM_HEIGHT),
     *              the path row) and the bottom wall 
     * 
     */
    
    //I am using the same variables, but for an entirely different purpose... call the cops.
    //Holy shit math/logic. I am going to screw something up.
    int left_column_right_bound = min((test_room.br.col - min_room_width), the_point.col);
    left_column_right_bound = max(left_column_right_bound, test_room.tl.col + 1);
    left_bound = rand() % (abs(left_column_right_bound - test_room.tl.col)) + test_room.tl.col;

    int right_column_left_bound = max((test_room.tl.col + min_room_width), the_point.col);
    right_column_left_bound = min(right_column_left_bound, test_room.br.col - 1);
    right_bound = rand() % (abs(test_room.br.col - right_column_left_bound)) + test_room.br.col;

    int top_row_lower_bound = min((test_room.br.row - min_room_height), the_point.row);
    top_row_lower_bound = max(top_row_lower_bound, test_room.tl.row + 1);
    upper_bound = rand() % (abs(top_row_lower_bound - test_room.tl.row)) + test_room.tl.row;

    int bottom_row_upper_bound = max((test_room.tl.row + min_room_height), the_point.row);
    bottom_row_upper_bound = min(bottom_row_upper_bound, test_room.br.row - 1);
    lower_bound = rand() % (abs(test_room.br.row - bottom_row_upper_bound)) + test_room.br.row;

    return Room(IntPoint(upper_bound, left_bound), IntPoint(lower_bound, right_bound));
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
    build_room(starting_point, br, 2);
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
            if ((point_is_beyond_bounds(potential_point)) || (!is_empty_space(potential_point)))
            {
                bad_direction = true;
                current_direction += 1;
            }
            tries++;
        } while((bad_direction) && tries < 4);
        
        current_point = potential_point;
    }
    return current_point;
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
    recursive_pblind_dungeon(target, deviation, squareness);
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
                                             int squareness)
{
    if (target == 0)
    {
        return;
    }

    Room current_room = main_dungeon.rooms[num_rooms];
    cout<<"Room points:"<<endl;
    cout<<current_room.tl;
    cout<<current_room.br<<endl;
    IntPoint point = rand_wall_block(current_room);

    IntPoint path_end = build_path(point, determine_which_wall(point));
    Room new_room = find_viable_room_space(path_end);
    if(new_room.tl.row == -1)
    {
        return;
    }
    build_room(new_room.tl, new_room.br, squareness);

    recursive_pblind_dungeon(target - 1, deviation, squareness);
    //build_path(point, 2);
    //main_dungeon.set_tile(point, PATH);
}


Dungeon DungeonBuilder::get_dungeon(){
	return main_dungeon;
}
