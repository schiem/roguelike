/**
 *  @file ENEMY.CPP
 *  @author Michael Yoder
 *  
 *  @section LICENSE
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
using namespace std;
Enemy::Enemy()
{
}

Enemy::Enemy(int _x, int _y, int _chunk_x, int _chunk_y, int _depth, EnemyType enemy) : Character(_x, _y, _chunk_x, _chunk_y, _depth)
{
    timer = 0;
    //determines if the character is good or evit, on a scale of 1-5 (5 is evil, 3 is passive)
    moral = enemy.moral;
    stats[HEALTH] = enemy.max_health;
    stats[ARMOR] = enemy.armor;
    stats[ATTACK] = enemy.base_attack;
    current_stats = stats;
    id = enemy.id;
    name = enemy.name;
    sight = enemy.sight;
    view = enemy.view;
    direction = 0;
    speed = enemy.speed;
    sprite = enemy.sprite;
    corpse = new Misc(IntPoint(x, y), enemy.corpse); 

    //is the enemy scared? if so, what direction and for how long
    spooked = false;
    direction_spooked = IntPoint(0, 0);
    time_spooked = 0;

    //generate the enemy's list of equipment and weapons
    vector<Equipment*> equip_list = generate_equipment(enemy.eq);
    for(int i = 0;i<equip_list.size();i++)
    {
        inventory.push_back(equip_list[i]);
    }
    Weapon* weapon = generate_weapon(vector<WeaponType>(enemy.wep));
    if(weapon != NULL)
    {
        inventory.push_back(weapon);
    }

}

void Enemy::run_ai(TilePointerMatrix &surroundings, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms)
{
    switch(id)
    {
        case 0:
            //kobolds
            aggressive_ai(surroundings, sur_chunk, sur_coords, char_list, delta_ms);
            break;
        case 1:
            //rabbits
            passive_ai(surroundings, sur_chunk, sur_coords, char_list, delta_ms);
            break;
        default:
            break;
    }
}

void Enemy::move(int x_change, int y_change)
{
    x+=x_change;
    y+=y_change;
    if(x<0)
    {
        x += CHUNK_WIDTH-1;
        chunk.col--;
    }
    if(x>=CHUNK_WIDTH)
    {
        x-=CHUNK_WIDTH - 1;
        chunk.col++;
    }
    if(y<0)
    {
        y += CHUNK_HEIGHT-1;
        chunk.row--;
    }
    if(y>=CHUNK_HEIGHT)
    {
        y -= CHUNK_HEIGHT-1;
        chunk.row++;
    }
}


int Enemy::get_sight()
{
    return sight;
}

int Enemy::get_id()
{
    return id;
}

IntPoint Enemy::get_sur_coords(IntPoint sur_chunk, IntPoint sur_coords, IntPoint _chunk, IntPoint _coords)
{
    IntPoint tl = get_abs(sur_chunk, sur_coords);
    IntPoint new_coords = get_abs(_chunk, _coords);
    return IntPoint(new_coords.row-tl.row, new_coords.col-tl.col);
}

IntPoint Enemy::get_next_step(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords)
{
    std::vector<IntPoint> path = a_star(cur_coords, goal, surroundings);
    if(path.size()>0)
    {
        IntPoint buffer_step = path[path.size() - 1];
        IntPoint next_step = IntPoint(buffer_step.row - cur_coords.row, buffer_step.col - cur_coords.col);
        return next_step;
    }
    else
    {
        return IntPoint(0, 0);
    }
}

std::vector<IntPoint> Enemy::a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
{
    std::vector<ATile> open;
    std::vector<ATile> closed;

    /* This is used so there is a consistant place for tiles
     * on the open list to point to.  Each tile's parent is
     * an int which corresponds to an index value for any tile
     * which has at some point been the "current tile"
     */ 
     std::vector<ATile> current_list;

    //The first tile is added to the open list
    open.push_back(ATile(-1, start));
    int cur_index;
    //while the goal hasn't been found
    while(is_in(goal, open) == -1)
    {
        if(open.size()<=0)
        {
            //if the open list is empty, stop
            break;
        }
        //set the current to the smallest f value
        int current_i = get_smallest_f(open);
        //set the "parent" index to the index of the current on the current_lsit
        cur_index = current_list.size();
        current_list.push_back(open[current_i]);

        //move the current from the open list to the closed list
        open.erase(open.begin() + current_i);
        closed.push_back(current_list[cur_index]);

        //loop through the surrounding tiles
        for(int i=current_list[cur_index].coords.row-1;i<=current_list[cur_index].coords.row+1;i++)
        {
            for(int j=current_list[cur_index].coords.col-1;j<=current_list[cur_index].coords.col+1;j++)
            {
                //make sure we're not about to operate on the current tile
                if(i!=current_list[cur_index].coords.row || j!=current_list[cur_index].coords.col)
                {
                    //check if this point is on the open list
                    int open_index = is_in(IntPoint(i, j), open);
                    
                    int y_move = (i-start.row) >= 0 ? (i-start.row) : ((i-start.row) * -1);
                    int x_move = (j-start.col) >= 0 ? (j-start.col) : ((j-start.col) * -1);
                    bool in_range = y_move <= sight && x_move <= sight;
                    bool is_good = i>=0 && j>=0 && i < surroundings.size() && j < surroundings[i].size();
                    //check if this point can be moved through, isn't on the open list, and isn't on the closed list and isn't out of range
                    if(in_range && is_good && surroundings[i][j]->can_be_moved_through && open_index == -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        ATile temp = ATile(cur_index, IntPoint(i, j));
                        temp.g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) && 
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i - 
                            current_list[cur_index].coords.row == 0) || (j - current_list[cur_index].coords.col == 0)));

                        temp.h = manhattan(IntPoint(i, j), goal);
                        temp.f = temp.h + temp.g;
                        open.push_back(temp);
                        
                    }
                    //check if it can be moved through, is on the open list, and isn't on the closed list
                    else if(surroundings[i][j]->can_be_moved_through && open_index != -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        //recalculate g to give a new f
                        int new_g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) && 
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i - 
                            current_list[cur_index].coords.row == 0) || (j - current_list[cur_index].coords.col == 0)));

                        //if the new g is lower, replace the old parent
                        if(new_g<open[open_index].g)
                        {
                            open[open_index].g = new_g;
                            open[open_index].parent = cur_index;
                            open[open_index].f = open[open_index].g + open[open_index].h;
                        }
                    }
                }
            }
        }
    }

    //get the path by starting at the goal
    //and iterating through parents until
    //we're back at the start.  The one before
    //start is the best move.
    std::vector<IntPoint> path;
    int index = is_in(goal, open);
    if(index != -1)
    {
        ATile current = open[index];
        while(current.parent != -1)
        {
            path.push_back(current.coords);
            current = current_list[current.parent];

        }
    }
    return path;
}

void Enemy::dump_matrix(TilePointerMatrix& map, IntPoint tl, IntPoint br)
{
    int tile;
    for(int i=tl.row; i<br.row;i++)
    {
        for(int j=tl.col; j<br.col;j++)
        {
            tile = map[i][j]->tile_id;
            switch(tile){
                case 1:
                    cout<<".";
                    break;
                case 2:
                    cout<<".";
                    break;
                case 4:
                    cout<<"#";
                    break;
                case 5:
                    cout<<"P";
                    break;
                case 6:
                    cout<<"M";
                    break;
                case 10:
                    cout<<"T";
                    break;
                case 11:
                    cout<<"#";
                    break;
                case 12:
                    cout<<"d";
                    break;
                case 13:
                    cout<<"u";
                    break;
                case 15:
                    cout<<"S";
                    break;
                case 16:
                    cout<<"~";
                    break;
                case 20:
                    cout<<"t";
                    break;
                case 21:
                    cout<<".";
                    break;
                default:
                    cout<<"0";
                    break;
            }
        }
        cout<<endl;
    }
    cout<<"-----------------------------------------------------"<<endl;
}

int Enemy::is_in(IntPoint point, std::vector<ATile> list)
{
    for(int i=0;i<list.size();i++)
    {
        if(list[i].coords.row == point.row && list[i].coords.col == point.col)
        {
            return i;
        }
    }
    return -1;
}

int Enemy::manhattan(IntPoint current, IntPoint goal)
{
    int dy = (current.row - goal.row) * 10;
    int dx = (current.col - goal.col) * 10;
    dx = (dx > 0) ? dx : -dx;
    dy = (dy > 0) ? dy : -dy;
    return dx + dy;
}

int Enemy::get_smallest_f(std::vector<ATile>& list)
{
    int smallest = list[0].f;
    int index = 0;
    ATile smallest_a = list[0];
    for(int i=1;i<list.size();i++)
    {
        if(list[i].f<smallest)
        {
            index = i;
            smallest = list[i].f;
        }
    }
    return index;
}

std::vector<Equipment*> Enemy::generate_equipment(std::vector<EquipType> equipment_list)
{
    std::vector<Equipment*> new_equipment;
    for(int i = 0; i<equipment_list.size();i++)
    {
        if(rand() % (5 + equipment_list[i].rarity) == 0)
        {
           new_equipment.push_back(new Equipment(IntPoint(y, x), equipment_list[i]));
        }
    }
    return new_equipment;
}

Weapon* Enemy::generate_weapon(std::vector<WeaponType> weapon_list)
{
    for(int i = 0; i<weapon_list.size();i++)
    {
        if(rand() % (5 + weapon_list[i].rarity) == 0)
        {
            return new Weapon(IntPoint(y, x), weapon_list[i]);
        }
    }
    return NULL;
}


/*------------------------------
 *   AI FUNCTIONS
 ------------------------------*/

bool Enemy::validate_target()
{
   return target != NULL && in_sight(target->get_coords(), target->get_chunk());
}

void Enemy::aggressive_ai(TilePointerMatrix &surroundings, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms)
{
    timer += delta_ms;
    IntPoint this_coords = get_sur_coords(sur_chunk, sur_coords, chunk, IntPoint(y, x));
    //If the timer > speed, then it is okay to act.
    while(timer > speed) {
        timer -= speed;
        //get a target
        if(validate_target() == false)
        {
            target = find_best_target(0, 1, char_list);
        }
        if(target != NULL)
        {
            //if we found a target, find the next step to get to it
            IntPoint target_coords = get_sur_coords(sur_chunk, sur_coords, target->get_chunk(), IntPoint(target->get_y(), target->get_x()));
            turn(target_coords - this_coords);
            IntPoint move_amount = get_next_step(target_coords, surroundings, this_coords);
            IntPoint next_step = IntPoint(this_coords.row + move_amount.row, this_coords.col + move_amount.col);
            if(next_step != target_coords)
            {
                //if we're not next to the target, step towards it
                if(rand() % 3 != 0) {
                    move(move_amount.col, move_amount.row);
                }
            }
            else
            {
                //attack it if we're next to it
                attack(target);
            }
        }
        else
        {
            //if we didn't find a target, meander
            int will_move = rand() % 5;
            int x_change = rand() % 3 - 1;
            int y_change = rand() % 3 - 1;
            int y_place = y_change + this_coords.row;
            int x_place = x_change + this_coords.col;
            bool in_range = y_place >= 0 && x_place >= 0 && y_place < surroundings.size() && x_place < surroundings[y_place].size();
            if(in_range && surroundings[y_change + this_coords.row][x_change + this_coords.col]->can_be_moved_through && will_move==0)
            {
                move(x_change, y_change);
            }
        }
    }
}



void Enemy::passive_ai(TilePointerMatrix &surroundings, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms)
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
        target = passive_best_target(0, 1, char_list);
        if(target != NULL && rand() % 5 == 0)
        {
            spooked = true;
            time_spooked = 0;
            IntPoint abs_coords = get_abs(chunk, IntPoint(y, x));
            IntPoint target_abs = get_abs(target->get_chunk(),  IntPoint(target->get_y(), target->get_x()));
            
            //some funky math that tells it which way to run
            
            direction_spooked = get_spooked(abs_coords, target_abs);
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
            if(surroundings[direction_spooked.row + sight + 1][direction_spooked.col + sight + 1]->can_be_moved_through)
            {
                move(direction_spooked.col, direction_spooked.row);
            }

        }
        else
        {
            int will_move = rand() % 5;
            int x_change = rand() % 3 - 1;
            int y_change = rand() % 3 - 1;
            if(surroundings[y_change + sight+1][x_change+sight+1]->can_be_moved_through && will_move==0)
            {
                move(x_change, y_change);
            }
        }
    }
}



Character* Enemy::find_best_target(int target_id, int selectability, std::vector<Character*> enemy_list)
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



Character* Enemy::passive_best_target(int target_id, int selectability, std::vector<Character*> enemy_list)
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

IntPoint Enemy::get_spooked(IntPoint abs_coords, IntPoint target_abs)
{
    float rise = abs_coords.row-target_abs.row;
    float run = abs_coords.col- target_abs.col;
    float slope = rise/run;
    float unsigned_slope = slope * (-1 * (slope < 0));
    int x_change = 0;
    int y_change = 0;

    //i just realized that I have no idea what this line does..
    //I think it was supposed to avoid a divide by 0 error, but clearly
    //does not.
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
    return IntPoint(y_change, x_change);
}

bool Enemy::in_sight(IntPoint coords, IntPoint chunk)
{
    IntPoint point = get_abs(coords, chunk);
    IntPoint center = get_abs(get_coords(), chunk);

    //get the distance.  sign matters
    IntPoint distance = point - center;
    
    //flag to check if it's in the distance
    bool in_distance = (distance.row * distance.row) + (distance.col * distance.col) <= sight*sight;

    //uses the direction the enemy is pointed and the width of the field of view
    //to establish the two angles that represent the upper and lower limits of what
    //the enemy can see
    IntPoint view_field = IntPoint(direction + (.5 * view), direction - (.5 * view));
    
    //convert them to rads
    double upper_bound = perc_to_rad(view_field.col);
    double lower_bound = perc_to_rad(view_field.row);

    //get the direction of the target
    double target_rads = coords_to_rad(distance);

    //check if the target's angle is in the acceptable range
    bool in_arc =  target_rads <= upper_bound && target_rads >= lower_bound;
    
    return in_arc && in_distance;
}

void Enemy::turn(IntPoint difference)
{
    int new_perc = coords_to_perc(difference);
    int turn = new_perc - direction;
    int new_direction = direction + turn;

    if(new_direction > 100)
    {
        new_direction -= 100;
    }
    else if(new_direction < 0)
    {
        new_direction += 100;
    }
    direction = new_direction;
}

std::vector<IntPoint> Enemy::sight_tiles()
{
    IntPoint coords = get_coords();
    return bresenham_arc(coords, sight, IntPoint(direction + (.5 * view), direction - (.5 * view)));
}

