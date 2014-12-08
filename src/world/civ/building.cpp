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
    BSpaceTree house = BSpaceTree(size.col-1, size.row-1, 3, 8);
    floor_plan.resize(size.row);
    floor = tiledef::WOOD_FLOOR;
    wall = tiledef::WOOD_WALL;
    for(int i=0;i<floor_plan.size();i++)
    {
        floor_plan[i].resize(size.col);
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
    /**for testing**/
    std::cout<<"New BUILDING!"<<std::endl;
    for(int i=0;i<rooms.size();i++)
    {
        std::cout<<"X: "<<rooms[i].tl.row<<", Y: "<<rooms[i].tl.col<<", Height: "<<rooms[i].height<<", Width: "<<rooms[i].width<<std::endl;
    }

}

void Building::rooms_from_tree(BSpaceNode* node)
{
    if(node->left == NULL && node->right == NULL)
    {
        rooms.push_back(Room(node->tl_x, node->tl_y, node->height, node->width));
   
    }
    else
    {
        if(node->left != NULL && node->right != NULL)
        {
            rooms_from_tree(node->left);
            rooms_from_tree(node->right);
        }
    }
}

void Building::rooms_to_floor()
{
    for(int i=0;i<rooms.size();i++)
    {
        Room room = rooms[i];
        for(int j=room.tl.row;j<=room.height + room.tl.row;j++)
        {
            for(int k=room.tl.col;k<=room.width + room.tl.col;k++)
            {
                if(j == room.tl.row || j == room.height+room.tl.row || k == room.tl.col || k == room.width+room.tl.col)
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
    add_random_door(bst.get_root());
}

void Building::connect_nodes(BSpaceNode* node)
{
    if(node->left != NULL && node->right != NULL)
    {
        //connect the two
        int x_coord;
        int y_coord;
        if(node->left->tl_y + node->left->height == node->right->tl_y)
        {
            do
            {
                x_coord = rand() % (node->left->width-2) + node->left->tl_x + 1; 
                y_coord = node->right->tl_y;
            } while(surrounding_walls(y_coord, x_coord) != 2);
        }
        else if(node->left->tl_x + node->left->width == node->right->tl_x)
        {
            do
            {
                y_coord = rand() % (node->left->height-2) + (node->left->tl_y + 1);
                x_coord = node->right->tl_x;
            } while(surrounding_walls(y_coord, x_coord) != 2);
        }
        floor_plan[y_coord][x_coord] = tiledef::DOOR;
        connect_nodes(node->left);
        connect_nodes(node->right);
    }
}


void Building::add_random_door(BSpaceNode* node)
{
    bool rand_x = rand() % 2;
    int x;
    int y;
    do
    {
        if(rand_x)
        {
            x = rand() % (size.col - 1) + 1;
            y = rand() % 2;
            if(y != 0)
            {
                y = size.row - 1;
            }
        }
        else
        {
            y = rand() % (size.row - 1) + 1;
            x = rand() % 2;
            if(x != 0)
            {
                y = size.col - 1;
            }
        }
    } while(surrounding_walls(y, x) != 2);
    floor_plan[y][x] = tiledef::DOOR;
}


int Building::surrounding_walls(int y, int x)
{   
    int wall_y = (floor_plan[y+1][x] == wall) + (floor_plan[y-1][x] == wall);
    int wall_x = (floor_plan[y][x+1] == wall) + (floor_plan[y][x-1] == wall);
    return wall_y + wall_x;
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
