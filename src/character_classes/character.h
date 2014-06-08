/**
 *  CHARACTER.H
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHARACTER_H
#define CHARACTER_H
#include <SDL/SDL.h>
#include <constants.h>
#include <terrain_defs.h>
#include <ASCII_Lib.h>
#include <def.h>
#include <iostream>
#include <int_point.h>
#include <item.h>
#include <vector>
class Character
{
    protected:
        //variables
        //Moral is a spectrum from 0-5, and represents
        //...morality? Basically, it's a good/evil thing
        //so you can see if something should attack/be attacked
        int moral;
        int current_health;
        int max_health;
        //current coordinates within dungeon
        int x;
        int y;
        int attack_dam;
        int armor;
        std::vector<Item*> inventory;
        std::vector<Item*> equipment;

        //Skills skill[];
        Tile sprite;
        Tile corpse;
        int depth;
        IntPoint chunk;
        Tile underfoot;
        Character* target;
        //methods

    public:
        Character();
        // what the heck are these parameters you guys??
        //lol who cares alan?
        Character(int, int, int, Tile, Tile, int, int, int, int, int);
        Character(int, int, int, int, int);
        bool is_alive() const;
        void take_damage(int);
        void attack(Character*);
        std::vector<Item*> get_inventory();
        std::vector<Item*> get_equipment();
        int get_x();
        int get_y();
        IntPoint get_chunk();
        int get_chunk_x();
        int get_chunk_y();
        Tile get_char();
        Tile get_corpse();
        int get_depth();
        Character* get_target();
        int get_max_hp();
        int get_cur_hp();
        int get_moral();
        int get_armor();
        void set_x(int);
        void set_y(int);
        void set_chunk(IntPoint);
        void set_chunk_x(int);
        void set_chunk_y(int);
        void set_depth(int);
        void set_target(Character*);
        void set_armor(int);
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
        Main_Character(int, int, int, Tile, Tile, int, int, int, int, int);
};

#endif
