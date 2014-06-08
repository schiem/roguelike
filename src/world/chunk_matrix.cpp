/*
 *  CHUNK_MATRIX.CPP
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

#include "chunk_matrix.h"

/**
 * @file chunk_matrix.cpp
 *
 * The primary data model and memory-loading bottleneck.
 *
 * The Chunk Matrix s a square matrix with a diameter that is given at
 * construction. The matrix is initialized using populate_initial, given central
 * chunk coordinates on the world map and a reference to the world map.
 *
 * For each of those chunks, that function calls the init() method, which will
 * by default construct the chunks if they haven't already been saved to the
 * filesystem or load them from the filesystem if they have.
 *
 * Every time the main character crosses a chunk boundary, shift_matrix will be
 * called. The chunk matrix will load in the proper chunks, creating or
 * deserializing chunks as needed.
 */

ChunkMatrix::ChunkMatrix() {

}

ChunkMatrix::ChunkMatrix(int _diameter, IntPoint center_chunk, MapTileMatrix& world_map) {
    diameter = _diameter;
    assert(diameter%2 != 0);

    model = vector<vector<Chunk> >(diameter, vector<Chunk>(diameter));
    int offset_dist_from_center = (diameter - 1) / 2;

    offset.row = center_chunk.row - offset_dist_from_center;
    offset.col = center_chunk.col - offset_dist_from_center;

    populate_initial(center_chunk, world_map);
}

/**
 * Populates the chunk matrix initially, affecting the internal model.
 * @param[in] center_chunk An IntPoint representing the central chunk in the matrix
 * @param[in] world_map The world map, passed by reference here to avoid copying.
 */
void ChunkMatrix::populate_initial(IntPoint center_chunk, MapTileMatrix& world_map) {
    int world_row, world_col;
    for(int row = 0; row < diameter; row++) {
        world_row = row + offset.row;
        for(int col = 0; col < diameter; col++) {
            world_col = col + offset.col;
            model[row][col].init(world_map[world_row][world_col],
                                world_row, world_col);
        }
    }
    pretty_print();
}

/**
 * Not actually as pretty as you might think, but prints a graphic of the chunk
 * matrix and its coordinates.
 */
void ChunkMatrix::pretty_print() {
    for(int row = 0; row < diameter; row++) {
        cout<<endl<<"--------------------------"<<endl;
        for(int col = 0; col<diameter; col++) {
            cout<<"|"<<model[row][col].get_world_loc();
        }
        cout<<"|";
    }
    cout<<endl<<"--------------------------"<<endl;
}

/**
 * Determines whether or not the given local coordinates on this small chunk map
 * are out of bounds.
 * @param[in] chunk_point an IntPoint representing a chunk's location on the
 * private "model" matrix.
 * @return True if the given point cannot lie on the chunk map.
 */
bool ChunkMatrix::out_of_bounds(IntPoint local_chunk_point) {
    return (local_chunk_point.row < 0 || local_chunk_point.row >= diameter ||
            local_chunk_point.col < 0 || local_chunk_point.col >= diameter);
}

/**
 * @return an IntPoint representing the amount by which the top left corner of
 * the ChunkMatrix is offset from the top left corner of the world.
 */
IntPoint ChunkMatrix::get_offset() {
    return offset;
}

/**
 * Set the offset (amount by which the top left corner of the ChunkMatrix
 * differs from the world's top left corner).
 * @param[in] point a point representing a location on the world map.
 */
void ChunkMatrix::set_offset(IntPoint point) {
    offset = point;
}


/**
 * @param abs_chunk_loc the absolute chunk coordinates on the world map.
 * @return a reference to the chunk from the local buffer.
 */
Chunk* ChunkMatrix::get_chunk_abs(IntPoint abs_chunk_loc) {
    IntPoint localized = IntPoint(abs_chunk_loc.row - offset.row,
                                  abs_chunk_loc.col - offset.col);
    if(out_of_bounds(localized)) {
        /*
        cout<<"Absolute point at "<<abs_chunk_loc.row<<", "<<abs_chunk_loc.col
            <<" was out of bounds: Localized to "<<localized.row<<", "
            <<localized.col<<endl<<"     offset = "<<offset<<endl;
            */

        return &model[0][0];
    }

    return &model[localized.row][localized.col];
}

/**
 * The same as the other get_chunk_abs, but with two ints instead of an
 * IntPoint.
 */
Chunk* ChunkMatrix::get_chunk_abs(int row, int col) {
    return get_chunk_abs(IntPoint(row, col));
}

/**
 * TODO update more code to use this instead of get_chunk_abs(main_char.get_y(),
 * main_char.get_x()) 
 * @return the central chunk in the model.
 */
Chunk* ChunkMatrix::get_center_chunk() {
    int rowcol = (diameter - 1) / 2;
    //cout<<"Center chunk = "<<rowcol<<endl;
    return &model[rowcol][rowcol];
}

/**
 * Shifts the contents of the chunk matrix.
 *
 * Will shift the chunk buffer around on the world map, agnostic of the
 * chunk buffer diameter. The concept of the chunk buffer is shown below.
 * Periods are unloaded chunks on the world map, and x's are chunks within the
 * chunk buffer (so, they are loaded). The buffer will move around, loading and
 * unloading chunks as needed. Serialization and deserialization should all be
 * handled in this function and the functions that it causes.
 *
 *    . . . . . .
 *    . . . . . .
 *    . . x x x .
 *    . . x x x .
 *    . . x x x .
 *    . . . . . .
 *
 * @param directions an IntPoint representing the direction to shift the matrix:
 * (1, 0) means one chunk to the right; (-1, -1) means one chunk to the left and
 * one chunk up; (0, -1) means 1 chunk up, etc.
 * @param world_map a reference to the world map.
 *
 */
void ChunkMatrix::shift_matrix(IntPoint directions, MapTileMatrix &world_map) {
    int world_row, world_col;

    if(directions.row == 1) {
        assert((offset.row + (diameter - 1)) < WORLD_HEIGHT);
        //For the topmost row, we will first have to serialize and clear.
        for(int col = 0; col < diameter; col++) {
            model[0][col].serialize();
        }

        //For everything but the last row, we will just copy the next row into the
        //current row.
        for(int row = 0; row < (diameter - 1); row++) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row + 1][col];
            }
        }

        //Create or deserialize new chunks for the last row.
        int row = diameter - 1;
        world_row = offset.row + diameter;
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }

    } else if (directions.row == -1) {
        assert(offset.row > 0);
        //For the last row, serialize.
        int row = diameter - 1;
        for(int col = 0; col < diameter; col++) {
            model[row][col].serialize();
        }

        //For everything but the first row, copy the previous row into the
        //current row.
        for(int row = (diameter - 1); row > 0; row--) {
            for(int col = 0; col < diameter; col++) {
                model[row][col] = model[row - 1][col];
            }
        }

        //Create or deserialize new chunks for the first row.
        world_row = offset.row - 1;
        for(int col = 0; col < diameter; col++) {
            world_col = offset.col + col;
            model[0][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    }

    if(directions.col == 1) {
        assert((offset.col + (diameter - 1)) < WORLD_WIDTH);

        //For the leftmost column, serialize and clear.
        for(int row = 0; row < diameter; row++) {
            model[row][0].serialize();
        }

        //For everything but the rightmost column, copy the right neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = 0; col < (diameter - 1); col++) {
                model[row][col] = model[row][col + 1];
            }
        }

        //Create or deserialize new chunks for the last column.
        int col = (diameter - 1);
        world_col = offset.col + diameter;
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][col].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    } else if (directions.col == -1) {
        assert(offset.col > 0);

        int col = diameter - 1;
        //For the rightmost column, serialize and clear.
        for(int row = 0; row < diameter; row++) {
            model[row][col].serialize();
        }

        //For everything but the leftmost column, copy the left neighbor into
        //the current column.
        for(int row = 0; row < diameter; row++) {
            for(int col = (diameter - 1); col > 0; col--) {
                model[row][col] = model[row][col - 1];
            }
        }

        //Create or deserialize new chunks for the first column.
        world_col = offset.col - 1;
        for(int row = 0; row < diameter; row++) {
            world_row = offset.row + row;
            model[row][0].init(world_map[world_row][world_col],
                                 world_row, world_col);
        }
    }

    offset.row += directions.row;
    offset.col += directions.col;
    cout<<"New offset: "<<offset.row<<", "<<offset.col<<endl;
    pretty_print();
}
