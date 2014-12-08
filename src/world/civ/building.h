/**
 *  @file BUILDING.H
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

#ifndef _BUILDING_H
#define _BUILDING_H

#include <binary_space.h>
#include <room.h>
#include <stdlib.h>
#include <defs.h>


/**
 * The class which is responsible for making buildings.
 */ 
class Building
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    private:
        /**
         * The location of the top left corner of the buildling.
         */
        IntPoint top_left;

        /**
         * The size of the building (height, width).
         */
        IntPoint size;

        /**
         * The number of floors in the house.
         */
        int num_floors;

        /**
         * Changes the way that the house is constructed.
         * Corresponds to an enum.  Really just a way so that
         * I can say "if(plan == PARALLEL){..." instead of 
         * needing to keep track of indexes.
         */
        //plan floor_plan;

        /**
         * The shape of the buildling.
         * Will be a circular or rectangular.  Almost all houses will
         * be circles.
         * Again, corresponds to an enum to access instead of 
         * index values.
         */
        //shape building_shape;

        /**
         * The tile that corresponds to what the wall is made out of.
         */
        Tile wall;

        /**
         * The tile that corresponds to what the floor is made out of.
         */
        Tile floor;

        /**
         * A list of the rooms.
         */
        std::vector<Room> rooms;

        /**
         * Holds the tiles of the house.
         */
        TileMatrix floor_plan; 
        
        /**
         * Generates the rooms and floor plan from a Binary Space Tree
         * generated in the height and width of the house.
         * Also adds...umm...doors.  That's what they're called.  Adds
         * Doors in between the rooms.
         */
        void house_from_bst(BSpaceTree bst);

        /**
         * Recursively turns the nodes stemming from a root node passed
         * in into room objects.
         */
        void rooms_from_tree(BSpaceNode* node);

        /**
         * Maps the rooms into a floor plan.
         */
        void rooms_to_floor();
        
        /**
         * Recursively adds doors in between every left-right pair of nodes.
         */
        void add_doors(BSpaceTree bst);

        /**
         * Actual recursive part of the add_doors() function.
         */
        void connect_nodes(BSpaceNode* node);

    public:
        /**
         * The default constructor for a building.
         */
        Building();

        /**
         * Constructor for buildings that actually builds the building.
         * At the moment, this does very little as I need to think of 
         * a good way to incorporate all of the variables that I need.
         * Basically, it's going to make a square house with several
         * rooms and a door.
         * @param _build_area The area to build on.
         * @param _start_point The coordinates of the top left corner in the build_area.
         * @param _size The size of the building (height and width).
         * @param _rooms The number of rooms in the building.
         * \todo Once seth figures out the whole depth thing, add a number of floors
         */
         Building(IntPoint _start_point, IntPoint _size);
        
};

#endif
