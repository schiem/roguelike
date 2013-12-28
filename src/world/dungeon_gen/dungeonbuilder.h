#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#define MAX_PATH_LENGTH 75
#define MIN_PATH_LENGTH 16
#define STD_ROOM_WIDTH 8
#define STD_ROOM_HEIGHT 6
//keep these two numbers even.
#define ROOM_WIDTH_DEV 2
#define ROOM_HEIGHT_DEV 2

#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h> //floor, ceil
#include <assert.h>

#include <int_point.h>
#include <room.h>
#include <terrain_defs.h>
#include <dungeon.h>
#include <ASCII_Lib.h>

using namespace std;

class DungeonBuilder
{
    friend ostream& operator<<(ostream&, const DungeonBuilder&);
    protected:
        static const int START_WIDTH=30;
        static const int START_HEIGHT=30;
        //variables
        int width;
        int height;
        int num_rooms;
        Dungeon main_dungeon;
        //methods
        bool rolled_over(int) const;
        bool is_empty_space(IntPoint) const;
        bool point_is_beyond_bounds(IntPoint) const;
        string edges_collide_with_something(Room&) const;
        int determine_which_wall(IntPoint) const;
        void set_wall_if_not_path(int, int);
        Room build_room(IntPoint, IntPoint);
        int get_wall_count(const Room&) const;
        IntPoint rand_wall_block(const Room&);
        IntPoint get_next_point(IntPoint, int) const;

    public:
        //DungeonBuilder();
        virtual void reset();
        //void print(SDL_Surface* ascii, SDL_Surface* screen, int color) const;
        Dungeon* get_dungeon();
};

#endif
