/**
 *  @file BUILDING.CPP
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

#include <building.h>

Building::Building(){
};


/* 
 * For now, the buildings will just be wood.  Mostly because I'm making
 * no progress sitting here thinking about how to do this, and so I should
 * just sit down and actually do it.  I can always add more later.
 */
Building::Building(IntPoint _start_point, IntPoint _size)
{
    top_left = _start_point;
    size = _size;
    BSpaceTree house = BSpaceTree(size.col, size.row, 3, 8);
    floor_plan.resize(size.row+1);
    floor = tiledef::WOOD_FLOOR;
    wall = tiledef::WOOD_WALL;
    for(int i=0;i<floor_plan.size();i++)
    {
        floor_plan[i].resize(size.col+1);
    }
    house_from_bst(house);
}

void Building::house_from_bst(BSpaceTree bst)
{
    //convert it into rooms
    rooms_from_tree(bst.get_root());
    //turn it into a floor_plan
    rooms_to_floor();
    //add some doors
    add_doors(bst);
}

void Building::rooms_from_tree(BSpaceNode* node)
{
    /**
    if(node->left == NULL && node->right == NULL)
    {
        */
        rooms.push_back(Room(node->tl_x, node->tl_y, node->height, node->width));
   
    /*}
    else
    {
        */
    if(node->left != NULL && node->right != NULL)
    {
        rooms_from_tree(node->left);
        rooms_from_tree(node->right);
    }
}

void Building::rooms_to_floor()
{
    for(int i=0;i<rooms.size();i++)
    {
        Room room = rooms[i];
        for(int j=room.tl.row;j<=room.height;j++)
        {
            for(int k=room.tl.col;k<=room.width;k++)
            {
                if(j == room.tl.row || j == room.height || k == room.tl.col || k == room.width)
                {
                    
                    floor_plan[j][k] = wall;
                }
                else
                {
                    if(floor_plan[j][k] != wall)
                    {
                        floor_plan[j][k] = floor;
                    }
                }
            }
        }
    }
}

void Building::add_doors(BSpaceTree bst)
{
    connect_nodes(bst.get_root());
}

void Building::connect_nodes(BSpaceNode* node)
{
    if(node->left != NULL && node->right != NULL)
    {
        //connect the two
        if(node->left->tl_y + node->left->height == node->right->tl_y)
        {
            int x_coord = rand() % node->left->width + node->left->tl_x; 
            floor_plan[node->right->tl_y][x_coord] = tiledef::DOOR;
        }
        else if(node->left->tl_x + node->left->width == node->right->tl_x)
        {
            int y_coord = rand() % node->left->height + node->left->tl_y;
            floor_plan[y_coord][node->right->tl_x] = tiledef::DOOR;
        }
        connect_nodes(node->left);
        connect_nodes(node->right);
    }
}


Tile Building::get_floor()
{
    return floor;
}

Tile Building::get_wall()
{
    return wall;
}

TileMatrix& Building::get_floor_plan()
{
    return floor_plan;
}

int Building::get_width()
{
    return size.col;
}

int Building::get_height()
{
    return size.row;
}

Tile Building::tile_at(int y, int x)
{
    return floor_plan[y][x];
}
