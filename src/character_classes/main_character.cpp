/**
 *  MAIN_CHARACTER.CPP
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

#include <character.h>
#include <iostream>

Main_Character::Main_Character() {

}

Main_Character::Main_Character(int _max_health, int _x, int _y,
                               Tile _sprite,  int _chunk_x, int _chunk_y , int _depth, int _morality, int _attack) :
    Character(_max_health, _x, _y,  _sprite, _chunk_x, _chunk_y, _depth, _morality, _attack){
}
