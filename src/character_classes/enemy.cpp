/**
 *  ENEMY.CPP
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
#define PATH_DEBUG 1

using namespace tiledef;
using namespace enemies;
using namespace std;
Enemy::Enemy()
{
}

Enemy::Enemy(int _max_health, int _x, int _y, Tile _sprite, int _chunk_x, int _chunk_y, int _depth, std::string _name) : Character(_max_health, _x, _y, _sprite, _chunk_x, _chunk_y, _depth)
{
    id = 1;
    name = _name;
}

Enemy::Enemy(EnemyType _enemy, int _x, int _y, int _chunk_x, int _chunk_y, int _depth) : Character(_x, _y, _chunk_x, _chunk_y, _depth)
{
    name = _enemy.name;
    max_health = _enemy.max_health;
    sprite = _enemy.sprite;
    id = _enemy.id;
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

void Enemy::run_ai(TileMatrix surroundings, Character* main_char)
{
    switch(id)
    {
        case 1:
            run_kobold_ai(surroundings, main_char);
            break;
        default:
            break;
    }
}

void Enemy::run_kobold_ai(TileMatrix& surroundings, Character* main_char)
{
    int radius = 20;
    //if the main_char is in the visible region
    if(main_char != NULL)
    {
        IntPoint main_coords = get_sur_coords(main_char->get_chunk(), IntPoint(main_char->get_y(), main_char->get_x()));
        IntPoint next_step = get_next_step(main_coords, surroundings);
        move(next_step.col-radius, next_step.row-radius);
    }
    else
    {
        int will_move = rand() % 5;
        int x_change = rand() % 3 - 1;
        int y_change = rand() % 3 - 1;
        if(surroundings[y_change + radius][x_change+radius].can_be_moved_through && will_move==0)
        {
            move(x_change, y_change);
        }
    }
}

int Enemy::get_id()
{
    return id;
}

IntPoint Enemy::get_sur_coords(IntPoint _chunk, IntPoint _coords)
{
    IntPoint tl = get_abs(chunk, IntPoint(y-20, x-20));
    IntPoint new_coords = get_abs(_chunk, _coords);
    return IntPoint(new_coords.row-tl.row, new_coords.col-tl.col);
}

IntPoint Enemy::get_next_step(IntPoint goal, TileMatrix& surroundings)
{
    std::vector<IntPoint> path = a_star(IntPoint(20, 20), goal, surroundings);
    if(path.size()>0)
    {
        return path[path.size()-1];
    }
    else
    {
        return IntPoint(0, 0);
    }
}

std::vector<IntPoint> Enemy::a_star(IntPoint start, IntPoint goal, TileMatrix& surroundings)
{
    std::vector<ATile> open;
    std::vector<ATile> closed;
    std::vector<ATile> current_list;
    open.push_back(ATile(-1, start));
    int cur_index;
    while(is_in(goal, open) == -1)
    {
        if(open.size()<=0)
        {
            break;
        }
        int current_i = get_smallest_f(open);
        cur_index = current_list.size();
        current_list.push_back(open[current_i]);
        open.erase(open.begin() + current_i);
        closed.push_back(current_list[cur_index]);
        if(PATH_DEBUG == 1)
        {
            dump_matrix(surroundings);
        }
        for(int i=current_list[cur_index].coords.row-1;i<=current_list[cur_index].coords.row+1;i++)
        {
            for(int j=current_list[cur_index].coords.col-1;j<=current_list[cur_index].coords.col+1;j++)
            {
                if(i!=current_list[cur_index].coords.row || j!=current_list[cur_index].coords.col)
                {
                    int open_index = is_in(IntPoint(i, j), open);
                    if(surroundings[i][j].can_be_moved_through && open_index == -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        ATile temp = ATile(cur_index, IntPoint(i, j));
                        temp.g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) &&
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.col == 0) || (j - current_list[cur_index].coords.col == 0)));

                        temp.h = manhattan(IntPoint(i, j), goal);
                        temp.f = temp.h + temp.g;
                        open.push_back(temp);
                        if(PATH_DEBUG == 1)
                        {
                            surroundings[temp.coords.row][temp.coords.col] = EMPTY;
                        }
                    }
                    else if(surroundings[i][j].can_be_moved_through && open_index != -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        int new_g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0)
                            && (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.col == 0) || (j - current_list[cur_index].coords.col == 0)));
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


void Enemy::dump_matrix(TileMatrix& map)
{
    int tile;
    for(int i=0; i<map.size();i++)
    {
        for(int j=0; j<map[i].size();j++)
        {
            tile = map[i][j].tile_id;
            switch(tile){
                case 1:
                    cout<<".";
                    break;
                case 11:
                    cout<<"T";
                    break;
                case 13:
                    cout<<"d";
                    break;
                case 16:
                    cout<<"S";
                    break;
                case 0:
                    cout<<"O";
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
