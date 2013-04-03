#ifndef DUNGEONBUILDER_H
#define DUNGEONBUILDER_H

#define MAX_PATH_LENGTH 100
#define MIN_PATH_LENGTH 16 
#define STD_ROOM_WIDTH 10
#define STD_ROOM_HEIGHT 10
#define ROOM_WIDTH_DEV 12
#define ROOM_HEIGHT_DEV 8
#include <iostream>
#include <string>
#include <stdlib.h>
#include <int_point.h>
#include <Room.h>
#include <terrain_defs.h>
#include <ncurses.h>
#include <Dungeon.h>
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

        //methods
		bool rolled_over(int) const;
		bool is_empty_space(IntPoint) const;
        bool point_is_beyond_bounds(IntPoint) const;
        int determine_which_wall(IntPoint) const;
        IntPoint find_viable_starting_point(int, int) const;
        Room build_room(IntPoint, IntPoint, int);
        Room find_viable_room_space(IntPoint) const;
        void build_start_room();
        int get_wall_count(const Room&) const;
        IntPoint rand_wall_block(const Room&);
        IntPoint get_next_point(IntPoint, int) const;
        IntPoint build_path(IntPoint, int);
        void recursive_pblind_dungeon(int, int, int, int);

	public:
		DungeonBuilder(int, int, int seed=time(NULL));
		int build_pblind_dungeon(int, int, int);
		void print() const;
};

#endif
