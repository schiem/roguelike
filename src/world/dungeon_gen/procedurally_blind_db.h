#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include "dungeonbuilder.h"

class ProcedurallyBlindDB : public DungeonBuilder 
{
    protected:
        //methods
        void build_start_room();
        Room find_viable_room_space(IntPoint) const;
        IntPoint find_viable_starting_point(int std_width, int std_height) const;
        IntPoint build_path(IntPoint start, int direction);
        void build_dungeon_recursive(int target);
    
    public:
        ProcedurallyBlindDB();
        ProcedurallyBlindDB(int _width, int _height, int _target_rooms, int seed=time(NULL));
        virtual void build_dungeon();
};

#endif
