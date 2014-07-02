/**
 *  @file OVERWORLD.H
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

#include <vector>
#include <spawner.h>
#include <int_point.h>
#include <bresenham.h>
#include <defs.h>
#include <chunk_layer.h>
#include <iostream>

namespace overworld_gen {
    typedef std::vector<std::vector<Tile> > TileMatrix;
    bool smoothing_pass(int threshold, Tile tile_type, int num);

    /**
     * Constructs an overworld of "land" type on this chunk. "Land" has dirt
     * and some trees.
     */
    void build_land_overworld(ChunkLayer& ground);

    /**
     * Constructs an ocean on this chunk. Every tile becomees a water tile.
     */
    void build_water_overworld(ChunkLayer& ground);

    /**
     * Constructs a beach on this chunk. Every tile becomes a type of sand
     * tile.
     */
    void build_beach_overworld(ChunkLayer& ground);

    /**
     * Constructs a forest on this chunk. Forests have dark grass floors,
     * thick trees, and tree-lined oases.
     */
    void build_forest_overworld(ChunkLayer& ground);
}

#endif
