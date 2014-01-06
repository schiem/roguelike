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
#define PATH_DEBUG 0

using namespace tiledef;
using namespace enemies;
using namespace std;
Enemy::Enemy()
{
}

Enemy::Enemy(int _max_health, int _x, int _y, Tile _sprite, int _chunk_x, int _chunk_y, int _depth, std::string _name, int _sight) : Character(_max_health, _x, _y, _sprite, _chunk_x, _chunk_y, _depth)
{
    id = 1;
    name = _name;
    sight = _sight;
}

Enemy::Enemy(EnemyType _enemy, int _x, int _y, int _chunk_x, int _chunk_y, int _depth) : Character(_x, _y, _chunk_x, _chunk_y, _depth)
{
    name = _enemy.name;
    max_health = _enemy.max_health;
    sprite = _enemy.sprite;
    id = _enemy.id;
    sight = _enemy.sight;
    speed = _enemy.speed;
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

void Enemy::run_ai(TileMatrix surroundings, Character* main_char, long delta_ms)
{
    timer += delta_ms;
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
    //If the timer > speed, then it is okay to act.
    if(timer > speed) {
        timer -= speed;
        //if the main_char is in the visible region
        if(main_char != NULL)
        {
            IntPoint main_coords = get_sur_coords(main_char->get_chunk(), IntPoint(main_char->get_y(), main_char->get_x()));
            IntPoint next_step = get_next_step(main_coords, surroundings);
            if(next_step != main_coords)
            {
                if(rand() % 6 != 0) {
                    move(next_step.col-(sight+1), next_step.row-(sight+1));
                }
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

int Enemy::get_id()
{
    return id;
}

int Enemy::get_sight()
{
    return sight;
}

/* Converts the chunk/x-y coordinates to coordinates in the enemies surroundings
 */
IntPoint Enemy::get_sur_coords(IntPoint _chunk, IntPoint _coords)
{
    IntPoint tl = get_abs(chunk, IntPoint(y-(sight+1), x-(sight+1)));
    IntPoint new_coords = get_abs(_chunk, _coords);
    return IntPoint(new_coords.row-tl.row, new_coords.col-tl.col);
}

IntPoint Enemy::get_next_step(IntPoint goal, TileMatrix& surroundings)
{
    std::vector<IntPoint> path = a_star(IntPoint(sight+1, sight+1), goal, surroundings);
    if(path.size()>0)
    {
        return path[path.size()-1];
    }
    else
    {
        return IntPoint(sight+1, sight+1);
    }
}

/* PRE: Assumes that the start and goal are within surroundings
 * POST: Returns a vector of IntPoints containing the appropriate path
 *
 * This calculates the best path between two coordinates on a given array of tiles
 * using the a* algorithm.  This is done by keeping track of an "open" and a "closed"
 * list of tiles.  The open list represents tiles that could potentially be looked at,
 * and the closed list is a list of tiles that have already been looked at.
 *
 * The current tile is the tile that is currently being considered, and every tile around
 * it is added to the open list, assuming that those tiles can be moved through and are
 * not on the closed list.  Each tile added to the open list is given a g, h, and f score,
 * and the curren tile becomes the parent.
 * G represents the distance from the start point to the tile, h (standing for heuristic)
 * is a measure from the tile to the goal, and f = h + g.  If a tile is on the open list,
 * then f is recalculated for the current tile.  If it is lower, then the current tile becomes
 * the new parent.
 *
 * Each loop, the current tile is the one on the open list with the shortest f score.  If the
 * goal is on the open list, or the open list is empty, then the algorithm is stopped.
 */

std::vector<IntPoint> Enemy::a_star(IntPoint start, IntPoint goal, TileMatrix& surroundings)
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

                    //check if this point can be moved through, isn't on the open list, and isn't on the closed list
                    if(surroundings[i][j].can_be_moved_through && open_index == -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        ATile temp = ATile(cur_index, IntPoint(i, j));
                        temp.g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0) &&
                            (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.col == 0) || (j - current_list[cur_index].coords.col == 0)));

                        temp.h = manhattan(IntPoint(i, j), goal);
                        temp.f = temp.h + temp.g;
                        open.push_back(temp);
                    }
                    //check if it can be moved through, is on the open list, and isn't on the closed list
                    else if(surroundings[i][j].can_be_moved_through && open_index != -1 && is_in(IntPoint(i, j), closed) == -1)
                    {
                        //recalculate g to give a new f
                        int new_g = current_list[cur_index].g + (14 * ((i - current_list[cur_index].coords.row != 0)
                            && (j - current_list[cur_index].coords.col != 0))) + (10 * ((i -
                            current_list[cur_index].coords.col == 0) || (j - current_list[cur_index].coords.col == 0)));

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
            if(PATH_DEBUG == 1)
            {
                surroundings[current.coords.row][current.coords.col] = EMPTY;
            }
            path.push_back(current.coords);
            current = current_list[current.parent];

        }
    }
    if(PATH_DEBUG == 1)
    {
        dump_matrix(surroundings);
    }
    return path;
}

/*
 * Little debugging thing to dump a tilematrix to cout
 */

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
                    cout<<".";
                    break;
                case 11:
                    cout<<"T";
                    break;
                case 12:
                    cout<<"#";
                    break;
                case 13:
                    cout<<"d";
                    break;
                case 14:
                    cout<<"u";
                    break;
                case 16:
                    cout<<"S";
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

/* PRE: I hate pre/post conditions.
 * POST: Returns an int containing the index if the element is present,
 * or -1 if it isn't.
 */

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

/* The manhattan heuristic.
 */

int Enemy::manhattan(IntPoint current, IntPoint goal)
{
    int dy = (current.row - goal.row) * 10;
    int dx = (current.col - goal.col) * 10;
    dx = (dx > 0) ? dx : -dx;
    dy = (dy > 0) ? dy : -dy;
    return dx + dy;
}

/*  PRE: It's a simple function.
 *  POST: Returns the smallest f value from a list.
 */

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
