/*
 *  @file TILESET.H
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

#ifndef TILESET_H
#define TILESET_H
#include <tile.h>

#include <unordered_map>

/**
 * A read-only singleton class, responsible for the tileset that is loaded.
 */
class Tileset
{
    private:
        static std::unordered_map<std::string, Tile> tileset;
        static Tileset* s_instance;
        Tileset();

    public:
        std::unordered_map<std::string, Tile>& get_tileset();
        static Tile get(std::string tilename);

        static Tileset* instance();
};

#endif
