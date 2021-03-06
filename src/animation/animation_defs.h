/**
 *  @file ANIMATION_DEFS.H
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

#ifndef __ANIMATION_DEF__H
#define __ANIMATION_DEF__H

#include <animation.h>
#include <actor.h>
#include <frame.h>
#include <bresenham.h>

/**
 * Makes an explosion animation!
 */
Animation construct_explosion(int x, int y, int chunk_x, int chunk_y, int size, Uint32 color);

#endif
