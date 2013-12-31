/**
 *  CORRUPTIBLE_PBLIND_DB.H
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

#ifndef _CORRUPTIBLE_PBLIND_DB_H
#define _CORRUPTIBLE_PBLIND_DB_H

#include <procedurally_blind_db.h>

class CorruptiblePBlindDB : public ProcedurallyBlindDB
{
    private:
        void corrupt_walls();
        void corrupt_corners(vector<IntPoint> corners);

    public:
        //CorruptiblePBlindDB();
        CorruptiblePBlindDB(int _width, int _height, int seed=time(NULL));
        void build_dungeon(int target, int deviation);
};

#endif
