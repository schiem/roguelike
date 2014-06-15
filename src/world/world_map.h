/**
 *  @file WORLD_MAP.H
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

#ifndef _WORLD_MAP_H
#define _WORLD_MAP_H

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <color_def.h>
#include <defs.h>
#include <constants.h>

/**
 * A representation of the game's world map.
 * The world map is finite and represented by a two-dimensional array, like
 * literally everything else in the game. The map takes care of its own
 * generation in its constructor, then the map is passed to the outside world by
 * a call to get_map().
 */
class WorldMap {

    /**
     * A self-explanatory typedef for the base model for this class.
     */
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:

        /**
         * The base model for this class. A two-dimensional matrix storing
         * MapTiles.
         *
         * @see MapTile
         */
        std::vector<std::vector<MapTile> > map;

        /**
         * Height, which is set to WORLD_HEIGHT in the constructor. Essentially
         * just makes the variable less verbose. May be removed.
         */
        int height;

        /**
         * Width, which is set to WORLD_WIDTH in the constructor. Essentially
         * just makes the variable less verbose. May be removed.
         */
        int width;

        /**
         * Determines whether a given row-column pair is out of bounds on the
         * world map matrix.
         *
         * @param row
         * @param col
         * @return a boolean denoting whether the point is out of bounds.
         */
        bool out_of_bounds(int row, int col);

        /**
         * Counts the number of occurences of the given MapTile type in the 8
         * tiles surrounding the given point.
         *
         * @param row
         * @param col
         * @param tile_type - the type of MapTile to query for
         * @return an integer result of the query.
         */
        int count_in_surrounding_tiles(int row, int col, MapTile tile_type);

        /**
         * Fills the world map (minus the ocean border) with randomly-placed
         * tiles, then performs the following transformation to all tiles on the
         * map:
         * \verbatim
           ?O?     ?O?
           O?O --> OOO
           ?O?     ?O?
         * \endverbatim
         *
         * @param border - the width of the ocean border surrounding the map.
         */
        void starting_noise(int border);

        /**
         * Will loop through the world map. On each tile, if the number of
         * surrounding tiles of the given type is greater or equal to the given
         * "threshold", there is a 1/(8 - num_surrounding_tiles) chance that the
         * tile in question will be changed to :tile_type:. This function is
         * relatively portable, and could be ported to any number of other
         * situations. When applied sequentially with different thresholds (as
         * in generate_land_mass()), will produce a passable world.
         */
        void smoothing_pass(MapTile tile_type, int threshold);

        /**
         * Generates the main land mass in the water by calling smoothing_pass
         * in sequence with different parameters on both land and water tiles.
         */
        void generate_land_mass();

        /**
         * Has a chance of setting a given tile to either a land or water tile.
         * This is for preliminary continent generation in the world.
         * @param row
         * @param col
         * @param mod - the inverse of the chance that the given tile will be
         * turned to water/land (for more_water=true/false respectively) @param
         * more_water - Should be set to 'true' if the given tile should be more
         * likely to be set to water, and 'false' if the given tile should be
         * more likely to be set to land.
         */
        void set_land_or_water(int row, int col, int mod, bool more_water);

        /**
         * Will generate an ocean border with the given width around the map.
         * @param border - the desired width of the border.
         */
        void ocean_borders(int border);

        /**
         * Generates beaches at the borders of all land and water tiles.
         */
        void generate_beaches();
    public:
        /**
         * The main constructor, which also generates the world's continents.
         */
        WorldMap();

        /**
         * Returns a reference to the world map.
         * @return a reference to the world map.
         */
        const std::vector<std::vector<MapTile> >& get_map();
};

#endif
