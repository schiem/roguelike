#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL/SDL.h>
#include <terrain_defs.h>
#include <ASCII_Lib.h>
#include <def.h>
#include <chunk.h>
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
        Tile sprite;
        int armor;
        int depth;
        Tile underfoot;
        //methods

    public:
        Character();
        Character(int, int, int, Tile, int);
        bool is_alive() const;
        void take_damage(int);
        void attack(int x_direction, int y_direction);
        int get_x();
        int get_y();
        Tile get_char();
        int get_depth();
        void set_x(int);
        void set_y(int);
        void set_depth(int);
        Tile* get_underfoot();
        //terrain get_surroundings

};

class Main_Character : public Character{
    protected:
        char name[];
        //inventory
        //equipment
   public:
        Main_Character();
        Main_Character(int, int, int, Tile, int);
};

#endif
