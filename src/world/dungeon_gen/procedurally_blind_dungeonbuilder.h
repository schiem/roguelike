#ifndef _PROCEDURALLY_BLIND_DUNGEONBUILDER_H
#define _PROCEDURALLY_BLIND_DUNGEONBUILDER_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "int_point.h"
#include "room.h"
#include "terrain_defs.h"
#include "dungeon.h"
#include "ASCII_Lib.h"
#include "dungeonbuilder.h"

class ProcedurallyBlindDungeonBuilder : public DungeonBuilder 
{
    private:
        //methods
        void build_start_room();
        Room find_viable_room_space(IntPoint) const;
        IntPoint find_viable_starting_point(int, int) const;
        IntPoint build_path(IntPoint, int);
        void build_dungeon_recursive(int, int, int);
    
    public:
        ProcedurallyBlindDungeonBuilder();
        ProcedurallyBlindDungeonBuilder(int, int, int seed=time(NULL));
        void reset();
        void build_dungeon(int, int, int);
};

#endif
