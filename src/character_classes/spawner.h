#ifndef SPAWNER_H
#define SPAWNER_H
#include <enemy.h>
#include <enemy_defs.h>
#include <ctime>

class Spawner
{
    protected:
        int x;
        int y;
        int depth;
        int chunk_x;
        int chunk_y;
        EnemyType enemy;

    public:
        Spawner();
        Spawner(int, int, int, int, int, EnemyType);
        bool should_spawn();
        Enemy spawn_creep();
        int get_x();
        int get_y();
};

#endif

