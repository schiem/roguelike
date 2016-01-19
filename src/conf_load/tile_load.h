/**
 *  @file CONF_LOAD.H
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

namespace tile_load {

    typedef struct {
        string tilename;
        int char_count;
        int tile_id;
        string color;
        bool corporeal;
        bool visible;
        bool opaque;
        bool seen;
        bool can_build_overtop;
    } tile;

    bool conf_exists();
    static int handle_ini_entry();
    void print_tile(tile&);
    void print_conf();
    void load_conf();

}

#endif
