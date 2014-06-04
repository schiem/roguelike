/**
 *  WORLD_MAP.H
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

#ifndef _WORLD_MAP_H
#define _WORLD_MAP_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <color_def.h>
#include <defs.h>

class WorldMap {
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:
        MapTileMatrix canvas;
        std::vector<std::vector<MapTile> > map;
        int height;
        int width;
        bool out_of_bounds(int, int);
        int count_in_surrounding_tiles(int, int, MapTile);
        void starting_noise(int);
        void generate_land_mass();
        void set_land_or_water(int, int, int, bool);
        void ocean_borders(int);
        void smoothing_pass(MapTile, int);
        void generate_beaches();
    public:
        WorldMap();
        WorldMap(int height, int width);
        const std::vector<std::vector<MapTile> >& get_map();
};

#endif
