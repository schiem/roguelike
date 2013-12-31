/**
 *  PROCEDURALLY_BLIND_DB.H
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

#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include <dungeonbuilder.h>

class ProcedurallyBlindDB : public DungeonBuilder
{
    protected:
        //methods
        void build_start_room();
        Room find_viable_room_space(IntPoint) const;
        IntPoint find_viable_starting_point(int std_width, int std_height) const;
        IntPoint build_path(IntPoint start, int direction);
        void build_dungeon_recursive(int target, int deviation);

    public:
        ProcedurallyBlindDB();
        ProcedurallyBlindDB(int _width, int _height, int seed=time(NULL));
        virtual void build_dungeon(int target, int deviation);
};

#endif
