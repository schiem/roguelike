#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "def.h"

struct MapTile {
    int char_count;
    int color;
    bool operator==(const MapTile& rhs) const {
        return this->char_count == rhs.char_count;
    }
};

namespace map_tile {
    extern MapTile MAP_DEFAULT;
    extern MapTile MAP_WATER;
    extern MapTile MAP_DIRT;
}

class WorldMap {
    private:
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

    public:
        WorldMap();
        WorldMap(int height, int width);
        std::vector<std::vector<MapTile> > get_map() const;
};
