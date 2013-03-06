#ifndef ROOM_H
#define ROOM_H
#include "int_point.h"

using namespace std;

class Room
{
    private:
        IntPoint tl;
        IntPoint br;

    public:
        Room(IntPoint, IntPoint);
};

#endif
