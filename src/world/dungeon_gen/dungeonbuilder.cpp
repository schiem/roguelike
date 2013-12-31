/**
 *  DUNGEONBUILDER.CPP
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

#include <dungeonbuilder.h>

using namespace tiledef;

/*
void DungeonBuilder::print(SDL_Surface* ascii, SDL_Surface* screen, int color) const {
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            drawChr(i, j, main_dungeon.get_tile(i, j).char_count, ascii, screen, color);
        }
    }
}

*/

/* PRE: Will be given :int given:, a number under 100.
 *
 * POST: Will perform a random number check between 1 and 100.
 * Will return true if result <= given; will return false if
 * result > given.
 */
bool DungeonBuilder::rolled_over(int given) const {
    int generated = rand() % 100;
    if(generated <= given) {
        return true;
    } else {
        return false;
    }
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the point is an empty
 * space.
 */
bool DungeonBuilder::is_empty_space(IntPoint point) const {
    return ((main_dungeon.get_tile(point) == BLOCK_WALL) || (main_dungeon.get_tile(point) == PATH));
}

/* PRE: Will be given :IntPoint point:
 *
 * POST: Will determine whether or not the given point is beyond the
 * bounds of the dungeon.
 */
bool DungeonBuilder::point_is_beyond_bounds(IntPoint point) const {
    //TODO This is set to 1 beyond the actual edge to make room for the border.
    //Consider having a better system.
    if((point.row < 1) or (point.row >= (main_dungeon.height - 1))) {
        return true;
    } else if((point.col < 1) or (point.col >= (main_dungeon.width - 1))) {
        return true;
    }

    return false;
}

/* PRE: Will be given :Room &r:
 * POST: Returns a string of 4 1s or 0s:
 *       Order: Top, right, bottom, left. A 1 means something collided,
 *       and a 0 means nothing collided.
 */
string DungeonBuilder::edges_collide_with_something(Room& r) const {
    string bin_string = "0000";
    for(int row = r.tl.row; row <= r.br.row; row++) {
        if (point_is_beyond_bounds(IntPoint(row, r.tl.col))) {
            bin_string[3] = '1';
        } else if (main_dungeon.get_tile(row, r.tl.col) == ROOM_WALL) {
            bin_string[3] = '1';
        }

        if (point_is_beyond_bounds(IntPoint(row, r.br.col))) {
            bin_string[1] = '1';
        } else if (main_dungeon.get_tile(row, r.br.col) == ROOM_WALL) {
            bin_string[1] = '1';
        }
    }

    for(int col = r.tl.col; col <= r.br.col; col++) {
        if (point_is_beyond_bounds(IntPoint(r.tl.row, col))) {
            bin_string[0] = '1';
        } else if (main_dungeon.get_tile(r.tl.row, col) == ROOM_WALL) {
            bin_string[0] = '1';
        }

        if (point_is_beyond_bounds(IntPoint(r.br.row, col))) {
            bin_string[2] = '1';
        } else if (main_dungeon.get_tile(r.br.row, col) == ROOM_WALL) {
            bin_string[2] = '1';
        }
    }
    return bin_string;
}

/* PRE: Will be given :IntPoint point: that lies on the wall of a room.
 * POST: Will determine which wall of the room the point lies on based
 *       on the surrounding tiles.
 */
int DungeonBuilder::determine_which_wall(IntPoint point) const {
    int direction = 0;

    if(point.col == 0) {
        return 3;
    } else if (point.row == 0) {
        return 0;
    } else if (point.col == main_dungeon.width - 1) {
        return 1;
    } else if (point.row == main_dungeon.height - 1) {
        return 2;
    } else if((main_dungeon.get_tile(point.row, point.col - 1) == ROOM_WALL) or
       (main_dungeon.get_tile(point.row, point.col + 1) == ROOM_WALL)) {

        if (main_dungeon.get_tile(point.row - 1, point.col) == DIRT) {
            direction = 2;
        }

        else if (main_dungeon.get_tile(point.row + 1, point.col) == DIRT) {
            direction = 0;
        }
    } else if ((main_dungeon.get_tile(point.row - 1, point.col) == ROOM_WALL) or
        (main_dungeon.get_tile(point.row + 1, point.col) == ROOM_WALL)) {

        if (main_dungeon.get_tile(point.row, point.col - 1) == DIRT) {
            direction = 1;
        }

        else if (main_dungeon.get_tile(point.row, point.col + 1) == DIRT) {
            direction = 3;
        }
    }

    return direction;
}

/* PRE: Will be given :int a: and :int b:, representing a row and column.
 * POST: If that row and column is not currently a PATH tile, set it to a
 *       ROOM_WALL tile.
 */
void DungeonBuilder::set_wall_if_not_path(int a, int b)  {
    if(main_dungeon.get_tile(a, b).tile_id != 4) {
        main_dungeon.set_tile(a, b, ROOM_WALL);
    }
}
/* PRE: Will be given :IntPoint tl:, which represents the top-left corner,
 * :IntPoint br:, which represents the bottom-right corner, and
 * :int squareness:, which denotes how square the rooms will be.
 *
 * POST: Will draw a room on the dungeon array with the given parameters.
 */
Room DungeonBuilder::build_room(IntPoint tl, IntPoint br) {
    set_wall_if_not_path(tl.row, tl.col);
    set_wall_if_not_path(tl.row, br.col);
    set_wall_if_not_path(br.row, tl.col);
    set_wall_if_not_path(br.row, br.col);

    //draw top and bottom rows
    for(int i = tl.col + 1; i <= br.col - 1; i++) {
        set_wall_if_not_path(tl.row, i);
        set_wall_if_not_path(br.row, i);
    }
    //draw left and right ROOM_WALLs
    for(int i = tl.row + 1; i <= br.row - 1; i++) {
        set_wall_if_not_path(i, tl.col);
        set_wall_if_not_path(i, br.col);
    }

    for(int i = tl.row + 1; i <= br.row - 1; i++) {
        for(int j = tl.col + 1; j <= br.col - 1; j++) {
            main_dungeon.set_tile(i, j, DIRT);
        }
    }
    main_dungeon.rooms[num_rooms] = Room(tl, br);
    num_rooms++;
    /*
    cout<<"ROOM BUILT AT ROW "<<tl.row<<". NUM_ROOMS: "<<num_rooms<<endl;
    cout<<"ROOM "<<num_rooms - 1<<": "<<main_dungeon.rooms[num_rooms - 1].tl.row
        <<", "<<main_dungeon.rooms[num_rooms - 1].tl.col<<endl;
        */

    return Room(tl, br);
}

/* PRE: Dungeon must be initialized
 * POST: Resets the num_rooms and main_dungeon variables, effectively
 * cleaning the dungeon.
 */
void DungeonBuilder::reset() {
    num_rooms = 0;
    main_dungeon = Dungeon(width, height);
}

/* PRE: Will be given a Room object.
 * POST: Will return the original number of wall spaces in the room
 * (corners are not included).
 */
int DungeonBuilder::get_wall_count(const Room &R) const {
    return (R.br.row - R.tl.row - 1) * 2 + (R.br.col - R.tl.col - 1) * 2;
}

/*
 * PRE: Will be given a Room object.
 * POST: Will return a random wall block that lies on the circumference of the given room.
 */
IntPoint DungeonBuilder::rand_wall_block(const Room &current_room) {
    int height = (current_room.br.row - current_room.tl.row) - 1;
    int width = (current_room.br.col - current_room.tl.col) - 1;
    int path_from_side = rand() % (height + width) + 1;
    IntPoint point;
    if (path_from_side < height) {
        int a = rand() % 2;
        if (a == 0) {
            point.row = current_room.tl.row;
        } else {
            point.row = current_room.br.row;
        }
        point.col = rand() % width + current_room.tl.col + 1;
    } else {
        int a = rand() % 2;
        if (a == 0) {
            point.col = current_room.tl.col;
        } else {
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
IntPoint DungeonBuilder::get_next_point(IntPoint this_point, int direction) const {
    IntPoint next_point = this_point;
    switch (direction) {
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

Dungeon* DungeonBuilder::get_dungeon(){
    return &main_dungeon;
}
