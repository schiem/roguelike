#ifndef ENEMY_DEFS_H
#define ENEMY_DEFS_H

#include <terrain_defs.h>
#include <string>

struct EnemyType
{
    int id;
    std::string name;
    int max_health;
    Tile sprite;
    bool operator==(const EnemyType& rhs) const 
    {
        return this->id==rhs.id;
    }
};

namespace enemies
{
    extern EnemyType Kobold;
}

#endif
