/**
 *  BINARY_SPACE.H
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

#ifndef _BINARY_SPACE_H
#define _BINARY_SPACE_H

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>

/**
 * The node class for the binary space tree.
 */
class BSpaceNode
{
    public:
        int height;
        int width;
        int tl_x;
        int tl_y;
        BSpaceNode* left;
        BSpaceNode* right;
        BSpaceNode(int _tl_x, int _tl_y, int _width, int _height);
};

/**
 * The class for constructing binary space trees, used to randomly 
 * divide a space into similar sized rectangles.
 */
class BSpaceTree
{
    private: 
        BSpaceNode* root;
        int height;
        int width;
        int min_size;
        int max_size;
        void delete_all(BSpaceNode* node);
        bool split_node(BSpaceNode* node);
        void generate_tree(BSpaceNode* node);
    public:
        BSpaceTree(int _height, int _width, int _min_size, int _max_size);
        ~BSpaceTree();
        BSpaceNode* get_root();
};    

#endif
