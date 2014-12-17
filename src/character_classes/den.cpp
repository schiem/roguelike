/**
 *  @file DEN.CPP
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

Den::Den(int _x, int _y, TileMatrix _ground)
{
    x = _x;
    y = _y;
    ground = _ground;
}

Den::Den(int _x, int _y, Tile sprite)
{
    ground.resize(1);
    ground[0].resize(1);
    ground[0][0] = sprite;
    x = _x;
    y = _y;
}

int Den::get_x()
{
    return x;
}

int Den::get_y()
{
    return y;
}

TileMatrix Den::get_ground()
{
    return ground;
}
