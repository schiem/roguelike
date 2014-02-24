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

class Character;

class Item
{
    protected:
        std::string name;
        int weight;
        Tile sprite;


    public:
        Item();
        Item(int, Tile, std::string);
        int get_weight();
        Tile get_sprite();
        virtual void perform_action(int, Character*)=0;
};


class Equipment : public Item
{
    protected:
        //corresponds to the index in the Characters equipment
        int body_part;

        //0 = light, 1 = medium, 2 = heavy
        int type;

        //bonus gained
        int armor_class;
        //special effects (not sure how to do this yet)
    
    public:
        Equipment(int, Tile, std::string, int, int, int);
        Equipment(EquipType);
        void perform_action(int, Character*);
        void equip(Character*);
        void remove(Character*);
};



#endif
