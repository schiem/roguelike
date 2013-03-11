#include <Room.h>

Room::Room()
{

}
Room::Room(IntPoint _tl, IntPoint _br)
{
    tl = _tl;
    br = _br;
}
void Room::print() const
{
    cout<<"Top Left: "<<tl<<"; Bottom Right: "<<br<<endl;
}
ostream& operator<<(ostream &out, const Room &R)
{
    R.print();
    return out;
}
