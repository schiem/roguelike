#ifndef _WORLD_MAP_H
#define _WORLD_MAP_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <def.h>
#include <terrain_defs.h>

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
