/**
 *  @file ROOM.CPP
 *  @author Seth A. Yoder
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

#include <room.h>

Room::Room() {

}

Room::Room(const Room& r) {
    tl = r.tl;
    br = r.br;
}

Room::Room(IntPoint _tl, IntPoint _br) {
    tl = _tl;
    br = _br;
}

void Room::print() const {
    cout<<"Top Left: "<<tl<<"; Bottom Right: "<<br<<endl;
}

ostream& operator<<(ostream &out, const Room &R) {
    R.print();
    return out;
}

Room::~Room() {

}
