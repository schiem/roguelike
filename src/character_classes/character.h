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
        char sprite;
        int armor;
        Chunk* chunk;
        int depth;
        Tile underfoot;
        //methods
    public:
        Character();
        Character(int, int, int, int, Chunk&, int);
        bool is_alive() const;
        void move(int, int);
        void take_damage(int);
        void attack(int x_direction, int y_direction);
        int get_x_loc();
        int get_y_loc();
        int get_char();
        int get_depth();        
        int get_chunk_x();
        int get_chunk_y();
        void set_x(int);
        void set_y(int);
        Tile* get_underfoot();
        void update_dungeon(Chunk&);       
        //terrain get_surroundings

};

class Main_Character : public Character{
    protected:
        char name[];
        //inventory
        //equipment
   public:
        Main_Character();
        //Main_Character(const Main_Character& m_C);
        Main_Character(int, int, int, int, Chunk&, int);
        void perform_action_cont();
        void perform_action_press(SDLKey);
};

#endif
            

