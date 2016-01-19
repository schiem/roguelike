/**
 *  @file TILE_LOAD.H
 *  @author Michael Yoder, Seth Yoder
 *
 *
 *  @section LICENSE
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

#ifndef TILE_LOAD_H
#define TILE_LOAD_H

#include <string.h>
#include <tile.h>
#include <unordered_map>

namespace tile_load {

    typedef struct {
        std::unordered_map<std::string, Tile> tiledefs;
        std::unordered_map<std::string, int> colordefs;
    } tileset_colorset_t;

    static int handle_ini_entry();
    void print_tile(Tile&);
    std::unordered_map<std::string, Tile> load_conf();

}

#endif
