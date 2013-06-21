#ifndef _CORRUPTIBLE_PBLIND_DB_H
#define _CORRUPTIBLE_PBLIND_DB_H

#include "procedurally_blind_db.h"

class CorruptiblePBlindDB : public ProcedurallyBlindDB
{
    private:
        void corrupt_walls(Room&);
        void corrupt_corners(vector<IntPoint> corners);
        void corrupt_dungeon();

    public:
        CorruptiblePBlindDB();
        CorruptiblePBlindDB(int _width, int _height, int _target_rooms, int seed=time(NULL));
        void build_dungeon();
};

#endif
