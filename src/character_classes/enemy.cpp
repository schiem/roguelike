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
    //I need a radius variable for how big the surroundings should be
    //but I'm not quite sure where to put it.  For now, I'll just code
    //one in.
    if(main_char != NULL)
    {
        //IntPoint main_coords = get_sur_coords(main_char->
        //IntPoint next_step = get_next_step(
    }
    else
    {
        int radius = 20;
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
