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
#include <helper.h>


struct ATile
{
    //my own jenky-ass linked list
    int parent;
    IntPoint coords;
    int f;
    int g;
    int h;
    ATile(){
        f = -1;
        g = -1;
        h = -1;
        }
    ATile(int _parent, IntPoint _coords)
    {
        parent = _parent;
        coords = _coords;
        f = 0;
        g = 0;
        h = 0;
    }
};

class Enemy : public Character
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    protected:
        int id;
        std::string name;
        int sight;
        void run_kobold_ai(TileMatrix&, Character*);
        void move(int, int);
        IntPoint get_cur_coords(IntPoint, IntPoint);
        IntPoint get_next_step(IntPoint, TileMatrix&);
        int is_in(IntPoint, std::vector<ATile>);
        std::vector<IntPoint> a_star(IntPoint, IntPoint, TileMatrix&);
        int manhattan(IntPoint, IntPoint);
        int get_smallest_f(std::vector<ATile>&);
        IntPoint get_sur_coords(IntPoint, IntPoint);
    public:
        Enemy();
        Enemy(int, int, int, Tile, int, int, int, std::string, int);
        Enemy(EnemyType, int, int, int, int, int);
        void run_ai(TileMatrix, Character*);
        void set_depth(int);
        int get_id();
        int get_sight();
        void dump_matrix(TileMatrix&);
};

#endif
