#ifndef ENEMY_H
#define ENEMY_H
#define CHUNK_WIDTH 100
#define CHUNK_HEIGHT 50


#include <character.h>
#include <enemy_defs.h>
#include <string>


class Enemy : public Character {
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    protected:
        int id;
        int chunk_x;
        int chunk_y;
        std::string name;
        void run_kobold_ai(TilePointerMatrix);
        void move(int, int);

    public:
        Enemy();
        Enemy(int, int, int, Tile, int, int, int, std::string);
        Enemy(EnemyType, int, int, int, int, int);
        void run_ai(TilePointerMatrix);
        int get_chunk_x();
        int get_chunk_y();
};

#endif
