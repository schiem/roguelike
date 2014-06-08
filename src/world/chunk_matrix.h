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
        int diameter; /** The diameter of this matrix. */
        vector<vector<Chunk> > model;
        IntPoint offset; /** The amount by which the top left corner of the matrix is offset from the world. */
        IntPoint center;
        void populate_initial(IntPoint center_chunk, MapTileMatrix &world_map);

    public:
        ChunkMatrix();
        ChunkMatrix(int _diameter, IntPoint center_chunk, MapTileMatrix &world_map);
        void pretty_print();
        bool out_of_bounds(IntPoint local_chunk_point);
        IntPoint get_offset();
        void set_offset(IntPoint point);
        Chunk* get_chunk_abs(IntPoint abs_chunk_loc);
        Chunk* get_chunk_abs(int row, int col);
        Chunk* get_center_chunk();
        void shift_matrix(IntPoint directions, MapTileMatrix &world_map);
};

#endif
