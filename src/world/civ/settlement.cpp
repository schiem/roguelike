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
    //BSpaceTree div = BSpaceTree(height, width, 100, 200);
    //settlement_from_bst(div);
    generate_buildings();
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


//****************TESTING**********************//

void Settlement::generate_buildings()
{
    //for now, statically sized houses.  We'll revisit this later
    BSpaceTree houses = BSpaceTree(height, width, 12, 24);
    houses.dump_all(); 
    //just get the base leaves.
    std::vector<BSpaceNode*> house_nodes = houses.get_leaves(); 
    std::cout<<"The number of leaves in me is: "<<house_nodes.size()<<std::endl;
    for(int i=0;i<house_nodes.size();i++)
    {
        std::cout<<"I'm a root node, with the following properties: "<<std::endl;
        std::cout<<"X: "<<house_nodes[i]->tl_x<<", Y: "<<house_nodes[i]->tl_y<<", Width: "<<house_nodes[i]->width<<", Height: "<<house_nodes[i]->height<<std::endl;
        int rand_x = rand() % 2 + 1;
        int rand_y = rand() % 2 + 1;
        int new_x = rand_x + house_nodes[i]->tl_x;
        int new_y = rand_y + house_nodes[i]->tl_y;
        int new_height = house_nodes[i]->height - (rand() % 2 + rand_y + 1);
        int new_width = house_nodes[i]->width - (rand() % 2 + rand_x + 1);
        buildings.push_back(Building(new_x, new_y, new_height, new_width));
    }
}

std::vector<Building>& Settlement::get_buildings()
{
    return buildings;
}

