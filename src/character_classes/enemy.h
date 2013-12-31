/**
 *  ENEMY.H
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

#ifndef ENEMY_H
#define ENEMY_H
#define CHUNK_WIDTH 100
#define CHUNK_HEIGHT 50
#include <character.h>
#include <enemy_defs.h>
#include <string>
#include <vector>

class Enemy : public Character
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    protected:
        int id;
        int chunk_x;
        int chunk_y;
        std::string name;
        void run_kobold_ai(TileMatrix);
        void move(int, int);

    public:
        Enemy();
        Enemy(int, int, int, Tile, int, int, int, std::string);
        Enemy(EnemyType, int, int, int, int, int);
        void run_ai(TileMatrix);
        int get_chunk_x();
        int get_chunk_y();
        void set_depth(int);
        int get_id();
};

#endif
