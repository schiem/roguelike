/**
 *  @file CORRUPTIBLE_PBLIND_DB.CPP
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

#include <corruptible_pblind_db.h>

using namespace std;
using namespace tiledef;

CorruptiblePBlindDB::CorruptiblePBlindDB(int _width, int _height) :
    ProcedurallyBlindDB(_width, _height) {

}

void CorruptiblePBlindDB::corrupt_corners(vector<IntPoint> corners) {
    int vert = 0;
    int horiz = 0;
    int wall_blocks_found = 0;
    //push some corners in.
    //std::vector.size() returns an unsigned long...
    for(size_t j = 0; j < corners.size(); j++) {
        if (rolled_over(20)) {
            main_dungeon.set_tile(corners[j], EMPTY);
            return;
        }

        if (rolled_over(40)) {
            vert = 0; horiz = 0; wall_blocks_found = 0;
            //There are a number of ways I could go about doing this.
            //I'm going with the weirdest, because YOLO.
            //This is for the first rule listed above corrupt_walls().

            //If the block below this point is a ROOM_WALL block...
            if((corners[j].row + 1 < main_dungeon.height) &&
                    (main_dungeon.get_tile(corners[j].row + 1, corners[j].col)
                     == ROOM_WALL)) {
                vert = 1;
                wall_blocks_found++;
            }
            if ((corners[j].row - 1 >= 0) &&
                    (main_dungeon.get_tile(corners[j].row - 1, corners[j].col)
                     == ROOM_WALL)) {
                vert = -1;
                wall_blocks_found++;
            }
            if ((corners[j].col + 1 < main_dungeon.width) &&
                    (main_dungeon.get_tile(corners[j].row, corners[j].col + 1)
                     == ROOM_WALL)) {
                horiz = 1;
                wall_blocks_found++;
            }
            if ((corners[j].col + 1 < main_dungeon.width) &&
                    (main_dungeon.get_tile(corners[j].row, corners[j].col - 1)
                     == ROOM_WALL)) {
                horiz = -1;
                wall_blocks_found++;
            }

            if (wall_blocks_found != 2) {
                return;
            } else {
                main_dungeon.set_tile(corners[j], EMPTY);
                main_dungeon.set_tile(corners[j].row + vert,
                        corners[j].col + horiz,
                        ROOM_WALL);
            }
        }
    }
}

void CorruptiblePBlindDB::corrupt_walls()
{
    //Loop through the rooms.
    for(int i = 0; i < num_rooms; i++) {
        Room& current_room = main_dungeon.rooms[i];
        IntPoint tl = current_room.tl;
        IntPoint tr = IntPoint(current_room.tl.row, current_room.br.col);
        IntPoint bl = IntPoint(current_room.br.row, current_room.tl.col);
        IntPoint br = current_room.br;
        vector<IntPoint> corners = vector<IntPoint>(4);
        corners[0] = tl; corners[1] = tr; corners[2] = bl; corners[3] = br;
        corrupt_corners(corners);
    }
}

void CorruptiblePBlindDB::build_dungeon(int target)
{
    reset();
    bool dungeon_is_awesome;
    build_start_room();
    int tries = 0;
    do {
        tries++;
        dungeon_is_awesome = true;
        build_dungeon_recursive(target);
        corrupt_walls();
        if (num_rooms < (target - 3)) {
            dungeon_is_awesome = false;
            main_dungeon = Dungeon(width, height);
            num_rooms = 0;
        }
    } while(!dungeon_is_awesome);
}
