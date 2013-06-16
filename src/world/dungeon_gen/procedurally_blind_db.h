#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include "dungeonbuilder.h"

class ProcedurallyBlindDB : public PoopBuilder 
{
    protected:
        //methods
        void build_start_room();
        Room find_viable_room_space(IntPoint) const;
        IntPoint find_viable_starting_point(int std_width, int std_height) const;
        IntPoint build_path(IntPoint start, int direction);
        void build_dungeon_recursive(int target, int deviation, int squareness);
    
    public:
        ProcedurallyBlindDB();
        ProcedurallyBlindDB(int _width, int _height, int seed=time(NULL));
        virtual void build_dungeon(int target, int deviation, int squareness);
};

#endif
