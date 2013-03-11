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
};

#endif
