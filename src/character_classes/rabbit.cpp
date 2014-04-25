/**
 *  RABBIT.CPP
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

Rabbit::Rabbit()
{
}

Rabbit::Rabbit(int _x, int _y, int _chunk_x, int _chunk_y, int _depth) : Enemy(_x, _y, _chunk_x, _chunk_y, _depth)
{
    //moral of 3 = passive
    moral = 3;
    current_health = 10;
    max_health = 10;
    attack_dam = 0;
    armor = 0;
    id = 2;
    sight = 15;
    speed = (rand() % 20) + 50;
    sprite = RABBIT;
    corpse = RABBIT_CORPSE;
    timer = 0;
    spooked = false;
    direction_spooked = IntPoint(0, 0);
    time_spooked = 0;
}

void Rabbit::run_ai(TileMatrix surroundings, std::vector<Character*> char_list, long delta_ms)
{
    timer += delta_ms;
    //If the timer > speed, then it is okay to act.
    while(timer > speed) {
        timer -= speed;
        
        //if it's spooked, have the chance to unspook
        if(spooked && rand() % (20 - time_spooked) == 0)
        {
            spooked = false;
        }
       
       //look for a nearby scary thing
        //if one is noticed, spook for the next turn
        target = find_best_target(0, 1, char_list);
        if(target != NULL && rand() % 2 == 0)
        {
            spooked = true;
            time_spooked = 0;
            IntPoint abs_coords = get_abs(chunk, IntPoint(y, x));
            IntPoint target_abs = get_abs(target->get_chunk(),  IntPoint(target->get_y(), target->get_x()));
            float rise = abs_coords.row-target_abs.row;
            float run = abs_coords.col- target_abs.col;
            float slope = rise/run;
            float unsigned_slope = slope * (-1 * (slope < 0));
            int x_change = 0;
            int y_change = 0;
            if(run == 0)
            {
                unsigned_slope = 2;
            }
            if(unsigned_slope < 1)
            {
                x_change = 0 - (run < 0) + (run > 0);
            }
            else if(unsigned_slope > 1)
            {
                y_change = 0 - (rise < 0) + (rise > 0);
            }
            else if(unsigned_slope == 1)
            {
                x_change = 0 - (run < 0) + (run > 0);
                y_change = 0 - (rise < 0) + (rise > 0);
            }

            direction_spooked = IntPoint(y_change, x_change);
        }

        if (spooked)
        {
            //try to move in the direction that it's spooked
            time_spooked += 1;
            if(rand() % 5 == 0)
            {
                direction_spooked.row = direction_spooked.row + (((rand() % 3) - 1) * (direction_spooked.row == 0)); 
                direction_spooked.col = direction_spooked.col + (((rand() % 3) - 1) * (direction_spooked.col == 0));
            }
            if(surroundings[direction_spooked.row + sight + 1][direction_spooked.col + sight + 1].can_be_moved_through)
            {
                cout<<"Direction: "<<IntPoint(direction_spooked.row, direction_spooked.col)<<endl;
                move(direction_spooked.col, direction_spooked.row);
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

Character* Rabbit::find_best_target(int target_id, int selectability, std::vector<Character*> enemy_list)
{
    Character* best = NULL;
    for(int i=0; i<enemy_list.size(); i++)
    {
        if(enemy_list[i]->get_moral() != 3)
        {
            if(best == NULL)
            {
                best = enemy_list[i];
            }
            else
            {
                if((unsigned int)(enemy_list[i]->get_moral() - moral) < (unsigned int)(best->get_moral() - moral))
                {
                    best = enemy_list[i];
                }
            }
        }
    }
    return best;
}
