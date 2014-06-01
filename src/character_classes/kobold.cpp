/**
 *  KOBOLD.CPP
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

#include <enemy.h>

using namespace tiledef;
using namespace equipment;


Kobold::Kobold()
{
}

Kobold::Kobold(int _x, int _y, int _chunk_x, int _chunk_y, int _depth) : Enemy(_x, _y, _chunk_x, _chunk_y, _depth)
{
    moral = 5;
    current_health = 20;
    max_health = 20;
    attack_dam = 2;
    armor = 0;
    id = 1;
    sight = 20;
    speed = 100;
    sprite = KOBOLD;
    corpse = KOBOLD_CORPSE; 
    EquipType eq[1] = {boots};
    vector<Equipment*> equip_list = generate_equipment(vector<EquipType>(&eq[0], &eq[0]+1));
    for(int i = 0;i<equip_list.size();i++)
    {
        cout<<"I have an item, and it's "<<equip_list[i]->get_name()<<endl;
        inventory.push_back(equip_list[i]);
    }
}

void Kobold::run_ai(TileMatrix surroundings, std::vector<Character*> char_list, long delta_ms)
{
    timer += delta_ms;
    //If the timer > speed, then it is okay to act.
    while(timer > speed) {
        timer -= speed;
        //get a target
        target = find_best_target(0, 1, char_list);

        if(target != NULL)
        {
            IntPoint target_coords = get_sur_coords(target->get_chunk(), IntPoint(target->get_y(), target->get_x()));
            IntPoint next_step = get_next_step(target_coords, surroundings);
            if(next_step != target_coords)
            {
                if(rand() % 3 != 0) {
                    move(next_step.col-(sight+1), next_step.row-(sight+1));
                }
            }
            else
            {
                attack(target);
            }
        }
        else
        {
            int will_move = rand() % 5;
            int x_change = rand() % 3 - 1;
            int y_change = rand() % 3 - 1;
            if(surroundings[y_change + sight+1][x_change+sight+1].can_be_moved_through && will_move==0)
            {
                move(x_change, y_change);
            }
        }
    }
}

Character* Kobold::find_best_target(int target_id, int selectability, std::vector<Character*> enemy_list)
{
    Character* best = NULL;
    for(int i=0; i<enemy_list.size(); i++)
    {
        if(enemy_list[i]->get_moral() > target_id - selectability && enemy_list[i]->get_moral() < target_id + selectability)
        {
            if(best == NULL)
            {
                best = enemy_list[i];
            }
            else
            {
                if((unsigned int)(enemy_list[i]->get_moral() - target_id) < (unsigned int)(best->get_moral() - target_id))
                {
                    best = enemy_list[i];
                }
            }
        }
    }
    return best;
}
