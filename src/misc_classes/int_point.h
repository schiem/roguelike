/**
 *  INT_POINT.H
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

#ifndef INT_POINT_H
#define INT_POINT_H
#include <iostream>

using namespace std;

class IntPoint
{
    friend ostream& operator<<(ostream&, const IntPoint&);
    private:
        void print() const;
    public:
        int col;
        int row;
        IntPoint();
        IntPoint(int _row, int _col);
        bool operator==(const IntPoint&);
        bool operator!=(const IntPoint&);
        IntPoint operator+(const int&);
        IntPoint operator+(const IntPoint&);
        IntPoint operator-(const int&);
        IntPoint operator-(const IntPoint&);
};

#endif
