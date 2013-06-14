#ifndef ROOM_H
#define ROOM_H
#include <int_point.h>
#include <iostream>

using namespace std;

class Room
{
    friend ostream& operator<<(ostream&, const Room&);
    private:
        void print() const;

    public:
        IntPoint tl;
        IntPoint br;
        Room();
        Room(const Room&);
        Room(IntPoint, IntPoint);
};

#endif
