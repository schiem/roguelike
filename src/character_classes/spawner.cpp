#include <spawner.h>

Spawner::Spawner()
{
    }

Spawner::Spawner(int _x, int _y, int _depth, int _chunk_x, int _chunk_y, EnemyType _enemy)
{
    x = _x;
    y = _y;
    depth = _depth;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
    enemy = _enemy;
}

bool Spawner::should_spawn()
{
    //TODO: put in a switch{case} for this
    return (rand() % 100 == 0);
}

Enemy Spawner::spawn_creep()
{
    return Enemy(enemy, x, y, chunk_x, chunk_y, depth);
}

int Spawner::get_x()
{
    return x;
}

int Spawner::get_y()
{
    return y;
}

