#ifndef CHARACTER_H
#define CHARACTER_H
#include <ncurses.h>
#include <terrain_defs.h>

class Character
{
	protected:
	//variables
	int current_health;
	int max_health;
	//current coordinates within dungeon
	int x;
	int y;
    //Skills skill[];
    char sprite;
	int armor;
    //methods

	public:
		Character(int, int, int, char);
        void display_character();
		bool is_alive() const;
		void move(int, int);
		void take_damage(int);
        void attack(int x_direction, int y_direction);
        int get_x_loc();
        int get_y_loc();
        char get_char();
        //terrain get_surroundings

};

class Main_Character : public Character{
    protected:
        char name[];
        //inventory
        //equipment
        //dungeon
   public:
        Main_Character(int, int, int, char);
        void perform_action(char);
};

#endif
            

