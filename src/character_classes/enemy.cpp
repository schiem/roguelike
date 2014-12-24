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

Enemy::Enemy(int _x, int _y, int _depth, EnemyType enemy) : Character(_x, _y, _depth)
{
    timer = 0;
    //determines if the character is good or evit, on a scale of 1-5 (5 is evil, 3 is passive)
    moral = enemy.moral;
    stats = enemy.stats;
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

Enemy::~Enemy()
{
    for(int i=0;i<inventory.size();i++)
    {
        delete inventory[i];
    }
    for(int i=0;i<equipment.size();i++)
    {
        delete equipment[i];
    }

    //delete corpse;
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
   return target != NULL && in_sight_range(target->get_coords(), target->get_chunk());
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

IntPoint Enemy::get_opposite(IntPoint abs_coords, IntPoint target_abs)
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

bool Enemy::in_sight_range(IntPoint _coords, IntPoint _chunk)
{
    IntPoint point = get_abs(_chunk, _coords);
    IntPoint center = get_abs(chunk, get_coords());

    //get the distance.  sign matters
    IntPoint distance = point - center;
    
    //flag to check if it's in the distance
    return (distance.row * distance.row) + (distance.col * distance.col) <= sight*sight;
}

bool Enemy::in_sight(IntPoint _coords, IntPoint _chunk)
{
    IntPoint point = get_abs(_chunk, _coords);
    IntPoint center = get_abs(chunk, get_coords());

    //get the distance.  sign matters
    IntPoint distance = point - center;
    
    //flag to check if it's in the distance
    bool in_distance = (distance.row * distance.row) + (distance.col * distance.col) <= sight*sight;

    //uses the direction the enemy is pointed and the width of the field of view
    //to establish the two angles that represent the upper and lower limits of what
    //the enemy can see
    IntPoint view_field = get_fov(); 
    
    //convert them to radians
    double upper_bound = perc_to_rad(view_field.row);
    double lower_bound = perc_to_rad(view_field.col);

    //get the direction of the target
    double target_rads = coords_to_rad(distance);

    //check if the target's angle is in the acceptable range
    bool in_arc = target_rads <= upper_bound && target_rads >= lower_bound;
    
    //check to make sure there's not something in the way
    
    
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
    std::vector<IntPoint> arc =  bresenham_arc(coords, sight, get_fov());
    std::vector<IntPoint> points;
    for(int i =0;i<arc.size();i++)
    {
        std::vector<IntPoint> line_points;
        line_points = bresenham_line(coords, arc[i]);
        for(int j=0;j<line_points.size();j++)
        {
            points.push_back(line_points[j]);
        }
    }
    return points;
}

IntPoint Enemy::get_fov()
{
    int upper = direction + (.5 * view);
    int lower = direction - (.5 * view);
    return IntPoint(upper, lower);
}
