#ifndef CHARACTER_H
#define CHARACTER_H

using namespace std;

class Character
{
	private:
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
		bool is_alive() const;
		void move(int, int);
		void take_damage(int);
		Character(int, int, int);
        void attack(int x_direction, int y_direction);
        int get_locaction();
        //terrain get_surroundings

};
#endif
