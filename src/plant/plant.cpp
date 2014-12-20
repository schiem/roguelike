/**
 *  @file PLANT.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
 *  This file is part of ROGUELIKETHING.
 *`
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

#include <plant.h>
/**
Plant::Plant(int _x, int _y, Tile _sprite)
{
    x = _x;
    y = _y;
    sprite = _sprite;
}
*/
Plant::Plant(int _x, int _y, PlantType plant)
{
    x = _x;
    y = _y;
    sprites = plant.sprites;
}

Plant::Plant()
{
}

int Plant::get_x()
{
    return x;
}

int Plant::get_y()
{
    return y;
}

Tile* Plant::get_sprite(int y, int x)
{
    return &sprites[y][x];
}

TileMatrix* Plant::get_sprites()
{
    return &sprites;
}

IntPoint Plant::get_coords()
{
    return IntPoint(y, x);
}
