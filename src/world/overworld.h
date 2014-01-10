/**
 *  OVERWORLD.H
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

#ifndef _OVERWORLD_H
#define _OVERWORLD_H

#include <terrain_defs.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <spawner.h>
#include <int_point.h>
#include <bresenham.h>
class Overworld
{
    typedef std::vector<std::vector<Tile> > TileMatrix;

    private:
        TileMatrix ground;
        Spawner spawner;
        bool smoothing_pass(int, Tile, int);
    public:
        bool has_layer_below;
        IntPoint down_stair;
        int width;
        int height;
        Tile* get_tile(int, int) ;
        void set_tile(int, int, Tile*);
        Overworld(int, int, bool, MapTile);
        Overworld();
        void build_land_overworld();
        void build_water_overworld();
        void build_beach_overworld();
        void build_forest_overworld();
        std::vector<std::vector<Tile> >& get_ground();
        Spawner get_spawner();

};

#endif
