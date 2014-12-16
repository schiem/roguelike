/**
 *  @file BLOCK.CPP
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

#include <block.h>

Block::Block() {

}

Block::Block(const Block& r) {
    tl = r.tl;
    height = r.height;
    width = r.width;
    buildings = r.buildings;
}

Block::Block(int tl_x, int tl_y, int _height, int _width)
{
    tl = IntPoint(tl_y, tl_x);
    height = _height;
    width = _width;
    
    generate_buildings();
}

void Block::generate_buildings()
{
    //for now, statically sized houses.  We'll revisit this later
    BSpaceTree houses = BSpaceTree(height, width, 15, 20);
    
    //just get the base leaves.
    std::vector<BSpaceNode*> house_nodes = houses.get_leaves(); 
    for(int i=0;i<house_nodes.size();i++)
    {
        int new_x = rand() % 3 + house_nodes[i]->tl_x;
        int new_y = rand() % 3 + house_nodes[i]->tl_y;
        int new_height = house_nodes[i]->height - rand() % 3;
        int new_width = house_nodes[i]->width - rand() % 3;
        //house_nodes[i]->resize(new_x, new_y, new_width, new_height);
        //THE BUILDINGS COORDINATES ARE RELATIVE TO THE BLOCK
        //BUILDINGS CURRENTLY HAVE A HARDCODED ROOM SIZE
        buildings.push_back(Building(IntPoint(new_y, new_x), IntPoint(new_height, new_width)));
    }
}

std::vector<Building>& Block::get_buildings()
{
    return buildings;
}
