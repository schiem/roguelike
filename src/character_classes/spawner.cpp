#include <spawner.h>

Spawner::Spawner()
{
    }

Spawner::Spawner(int _x, int _y, int _depth,  EnemyType _enemy)
{
    x = _x;
    y = _y;
    depth = _depth;
    enemy = _enemy;
}

bool Spawner::should_spawn()
{
    //TODO: put in a switch{case} for this
    return (rand() % 100 == 0);
}

Enemy Spawner::spawn_creep(int chunk_x, int chunk_y)
{
    return Enemy(enemy, x + 1, y + 1, chunk_x, chunk_y, depth);
}

int Spawner::get_x()
{
    return x;
}

int Spawner::get_y()
{
    return y;
}

