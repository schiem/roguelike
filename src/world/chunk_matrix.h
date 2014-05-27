/**
 *  CHUNK_MATRIX.H
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

#ifndef _CHUNK_MATRIX_H
#define _CHUNK_MATRIX_H

#include <stdlib.h>
#include <vector>

#include <chunk.h>
#include <constants.h>
#include <int_point.h>
#include <world_map.h>

using namespace std;

class ChunkMatrix
{
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    private:
        int diameter;
        vector<vector<Chunk> > model;
        IntPoint offset;
        IntPoint center;
        void populate_initial(IntPoint, MapTileMatrix&);

    public:
        ChunkMatrix();
        ChunkMatrix(int, IntPoint, MapTileMatrix&);
        bool out_of_bounds(IntPoint);
        vector<vector<Chunk> >& get_matrix();
        IntPoint get_offset();
        void set_offset(IntPoint);
        void set_chunk_abs(IntPoint, Chunk);
        Chunk* get_chunk_abs(IntPoint);
        Chunk* get_chunk_abs(int, int);
        void set_chunk(IntPoint, Chunk);
        Chunk* get_chunk(IntPoint);
        Chunk* get_center_chunk();
        void shift_matrix(IntPoint, MapTileMatrix&);

};

#endif
