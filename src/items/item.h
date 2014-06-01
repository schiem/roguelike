/**
 *  ITEM.H
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

#ifndef ITEM_H
#define ITEM_H
#include <terrain_defs.h>
#include <string>
#include <item_defs.h>
#include <int_point.h>

class Character;

static std::string BODY_PARTS[] = {"Head", "Torso", "Legs", "Feet", "Arms", "Hands", "Wielding"}; 

class Item
{
    protected:
        std::string name;
        int weight;
        IntPoint coords;
        Tile sprite;
        int rarity; 

    public:
        bool can_equip;
        bool can_use;
        bool can_wield;

        Item(IntPoint);
        Item(int, int, Tile, std::string, IntPoint);
        int get_weight();
        Tile* get_sprite();
        virtual void perform_action()=0;
        IntPoint get_coords();
        void set_coords(IntPoint);
        std::string get_name();
        int get_rarity();
};


class Equipment : public Item
{
    protected:
        //corresponds to the index in the Characters equipment
        /*****
           0
         54145
           1
          2 2
          3 3
        *****/ 
        int body_part;

        //0 = light, 1 = medium, 2 = heavy
        int type;

        //bonus gained
        int armor_class;
        //special effects (not sure how to do this yet)
    
    public:
        Equipment(int, int, Tile, std::string, IntPoint, int, int, int);
        Equipment(IntPoint, EquipType);
        void perform_action();
        int get_body_part();
        int get_armor();
};

class Weapon : public Item
{
    protected:
        //weapons are always wielded in slot 6 of the body part diagram

        //bonus gained
        int damage;
        
        //range of the weapon
        int range;

        //type
        //1 = piercing, 2 = slashing, 3 = bludgeoning
        int type;

        //special effects (not sure how to do this yet)
    
    public:
        Weapon(int, int, Tile, std::string, IntPoint, int, int);
        Weapon(IntPoint, WeaponType);
        void perform_action();
        int get_type();
        int get_damage();
};


#endif
