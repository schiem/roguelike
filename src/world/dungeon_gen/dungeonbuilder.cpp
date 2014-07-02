/**
 *  @file DUNGEONBUILDER.CPP
 *  @author Seth A. Yoder
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

#include <dungeonbuilder.h>

using namespace tiledef;


bool DungeonBuilder::rolled_over(int given) const {
    int generated = rand() % 100 + 1;
    if(generated < given) {
        return true;
    } else {
        return false;
    }
}

bool DungeonBuilder::is_empty_space(IntPoint point) const {
    return ((main_dungeon.get_tile(point) == BLOCK_WALL) || (main_dungeon.get_tile(point) == PATH));
}

bool DungeonBuilder::point_is_beyond_bounds(IntPoint point) const {
    if((point.row < 0) or (point.row >= main_dungeon.height)) {
        return true;
    } else if((point.col < 0) or (point.col >= main_dungeon.width)) {
        return true;
    }

    return false;
}

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

    return Room(tl, br);
}

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

void DungeonBuilder::set_wall_if_not_path(int row, int col)  {
    if(!(main_dungeon.get_tile(row, col) == PATH)) {
        main_dungeon.set_tile(row, col, ROOM_WALL);
    }
}

void DungeonBuilder::reset() {
    num_rooms = 0;
    main_dungeon = ChunkLayer(width, height);
}

ChunkLayer* DungeonBuilder::get_dungeon(){
    return &main_dungeon;
}
