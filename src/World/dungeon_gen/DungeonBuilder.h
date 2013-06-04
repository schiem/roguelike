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
#include <SDL/SDL.h>
#include <math.h> //floor, ceil

#include "int_point.h"
#include "Room.h"
#include "terrain_defs.h"
#include "Dungeon.h"
#include "ASCII_Lib.h"

using namespace std;

class DungeonBuilder
{
    friend ostream& operator<<(ostream&, const DungeonBuilder&);
	private:
        //variables
		int width;
		int height;
        int num_rooms;
        Dungeon main_dungeon;
		bool initialized;
        //methods
		bool rolled_over(int) const;
		bool is_empty_space(IntPoint) const;
        bool point_is_beyond_bounds(IntPoint) const;
        string edges_collide_with_something(Room&) const;
        int determine_which_wall(IntPoint) const;
        IntPoint find_viable_starting_point(int, int) const;
        void set_wall_if_not_path(int, int);
        Room build_room(IntPoint, IntPoint, int);
        Room find_viable_room_space(IntPoint) const;
        void build_start_room();
        int get_wall_count(const Room&) const;
        IntPoint rand_wall_block(const Room&);
        IntPoint get_next_point(IntPoint, int) const;
        IntPoint build_path(IntPoint, int);
        void recursive_pblind_dungeon(int, int, int);

	public:
		DungeonBuilder(int, int, int seed=time(NULL));
		int build_pblind_dungeon(int, int, int);
		void print(SDL_Surface* ascii, SDL_Surface* screen, int color) const;
		void initialize();
		bool is_initialized();
		Dungeon get_dungeon();
};

#endif
