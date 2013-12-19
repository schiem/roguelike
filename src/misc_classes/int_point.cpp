#include "int_point.h"

IntPoint::IntPoint() {
}

IntPoint::IntPoint(int _row, int _col) {
    row = _row;
    col = _col;
}

void IntPoint::print() const {
    cout<<"("<<row<<", "<<col<<")";
}

ostream& operator<<(ostream &out, const IntPoint &I) {
    I.print();
    return out;
}
