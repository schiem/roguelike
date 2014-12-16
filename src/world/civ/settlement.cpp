/**
 *  @file SETTLEMENT.CPP
 *  @author Michael and Seth Yoder
 *
 *  @section LICENSE
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

#include <settlement.h>

Settlement::Settlement(int chunk_x, int chunk_y, int _height, int _width)
{
    chunk = IntPoint(chunk_y, chunk_x);
    height = _height;
    width = _width;
    //first thing we need to do it divide it into city blocks
    //so we can have roads running through it.
    BSpaceTree div = BSpaceTree(height, width, 40, 100);
    settlement_from_bst(div);
}

void Settlement::settlement_from_bst(BSpaceTree& bst)
{
    blocks_from_bst(bst);
}

void Settlement::blocks_from_bst(BSpaceTree& tree)
{
    std::vector<BSpaceNode*> leaves = tree.get_leaves();
    for(int i=0;i<leaves.size();i++)
    {        
        int new_x = rand() % 3 + leaves[i]->tl_x;
        int new_y = rand() % 3 + leaves[i]->tl_y;
        int new_height = leaves[i]->height - rand() % 3;
        int new_width = leaves[i]->width - rand() % 3;
        blocks.push_back(Block(new_x, new_y, new_height, new_width));
    }
}

std::vector<Block>& Settlement::get_blocks()
{
    return blocks;
}
