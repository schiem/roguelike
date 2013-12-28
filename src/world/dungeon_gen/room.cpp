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
