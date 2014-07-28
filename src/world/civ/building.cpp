/**
 *  @file BUILDING.CPP
 *  @author Michael and Seth Yoder
 *
 *  @section LICENSE
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

Building::Building(){
};


/* 
 * For now, the buildings will just be wood.  Mostly because I'm making
 * no progress sitting here thinking about how to do this, and so I should
 * just sit down and actually do it.  I can always add more later.
 */
Building::Building(&TileMatrix _build_area, IntPoint _start_point, IntPoint _size, int _rooms)
{
    top_left = _start_point;
    size = _size;
    num_rooms = _rooms;
    generate(_build_area);
}

void Building::generate(&TileMatrix area)
{
    lay_foundation(area);
    make_rooms(area);
}

void Building::lay_foundation(&TileMatrix area)
{
    IntPoint bottom_right = top_left + size;
    for(int i=top_left.row;i<bottom_right.row;i++)
    {
        for(int j=top_left.col; bottom_right.col;j++)
        {
            bool is_wall = i==top_left.row || j==top_left.col || i == bottom_right.row - 1 || j == bottom_right.col - 1;
            if(is_wall)
            {
                area[i][j] = WOOD_WALL;
            }
            else
            {
                area[i][j] = WOOD_FLOOR;
            }
        }
    }
}

void Building::make_rooms(&TileMatrix area)
{
    //the area of the house
    int area = (size.row - 1) * (size.col - 1);

    //the ratio of rooms to area
    //this will be rounded down.  that's okay, it's really
    //only for giving a general idea of size/room anyway
    int ratio;
    
    /*
     * If our rooms are less than they're allowed to be,
     * remove them until they are the correct size.
     * If we don't have any rooms, then stahp er'thang.
     * This shouldn't happen, but I'm sure I'll end up
     * with a whole ton of houses that are just 2x2 empty
     * squares at some point.
     * The reason that the return is at the beginning is to avoid
     * /0 errors.
     */
    do {
        if(num_rooms <= 0) return;
        ratio = area/num_rooms;
        if(ratio < MIN_ROOM_AREA)
        {
            num_rooms = num_rooms - 1;
        }
    }
    while(ratio < MIN_ROOM_AREA);
    int outside_doors = rand() % 3;
    //rooms will always lead to the outside, subrooms will have one
    //door leading to a larger room.  There will be 0-1/2 * num_rooms
    //sub_rooms
    int sub_rooms = rand() % ((.5 * num_rooms) + 1);


