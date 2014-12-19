/**
 *  @file PLANT.H
 *  @author Michael Yoder
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

#ifndef _PLANT_H
#define _PLANT_H

#include <defs.h>
#include <int_point.h>

/**
 * The class which is used for plants, so that plants can grow and
 * whatnot.
 */

class Plant
{
    private:
        /**
         * X coordinate of the plant.
         */
        int x;
        
        /**
         * Y coordinate of the plant.
         */
        int y;
        
        /**
         * The character representing the plant.
         * TODO: Make this a 2d vector so that plants
         * can be as biiiiig as we want them to be.
         */
        Tile sprite;
    public:
        
        /**
         * Basic constructor.
         */
        Plant(int _x, int _y, Tile _sprite);
        
        /**
         * Empty constructor.
         */
        Plant();
        
        /**
         * Public accessor for the x coordinate.
         */
        int get_x();
        
        /**
         * Public accessor for the y coordinate.
         */
        int get_y();
        
        /**
         *  Public accessor for the sprite.
         */
        Tile* get_sprite();

        /**
         * Returns the coordinates as an IntPoint.
         */
        IntPoint get_coords();
};

#endif
