#include <enemy.h>

using namespace tiledef;
using namespace enemies;

Enemy::Enemy()
{
}

Enemy::Enemy(int _max_health, int _x, int _y, Tile _sprite, int _depth, int _chunk_x, int _chunk_y, std::string _name) : Character(_max_health, _x, _y, _sprite, _depth)
{
    id = 1;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
    name = _name;
}

Enemy::Enemy(EnemyType _enemy, int _x, int _y, int _chunk_x, int _chunk_y, int _depth) : Character(_x, _y, _depth)
{
    name = _enemy.name;
    max_health = _enemy.max_health;
    sprite = _enemy.sprite;
    id = _enemy.id;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
}

void Enemy::move(int x_change, int y_change)
{
    x+=x_change;
    y+=y_change;
    if(x<0)
    {
        x += CHUNK_WIDTH-1;
        chunk_x--;
    }
    if(x>=CHUNK_WIDTH)
    {
        x-=CHUNK_WIDTH;
        chunk_x++;
    }
    if(y<0)
    {
        y += CHUNK_HEIGHT-1;
        chunk_y--;
    }
    if(y>=CHUNK_HEIGHT)
    {
        y -= CHUNK_HEIGHT;
        chunk_y++;
    }
}

void Enemy::run_ai(TileMatrix surroundings)
{
    switch(id)
    {
        case 1:
            run_kobold_ai(surroundings);
            break;
        default:
            break;
    }
}

void Enemy::run_kobold_ai(TileMatrix surroundings)
{
    //I need a radius variable for how big the surroundings should be
    //but I'm not quite sure where to put it.  For now, I'll just code
    //one in.
    int radius = 20;
    int will_move = rand() % 5;
    int x_change = rand() % 3 - 1;
    int y_change = rand() % 3 - 1;
    if(surroundings[y_change + radius][x_change+radius].can_be_moved_through && will_move==0)
    {
        move(x_change, y_change);
    }
}


int Enemy::get_chunk_x()
{
    return chunk_x;
}

int Enemy::get_chunk_y()
{
    return chunk_y;
}

int Enemy::get_id()
{
    return id;
}
