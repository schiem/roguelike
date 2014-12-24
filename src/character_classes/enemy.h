/**
 *  @file ENEMY.H
 *  @Author Michael Yoder
 *
 *  @section LICENSE
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

#ifndef ENEMY_H
#define ENEMY_H
#include <character.h>
#include <defs.h>
#include <constants.h>
#include <string>
#include <vector>
#include <helper.h>
#include <constants.h>
#include <math_helper.h>
#include <bresenham.h>

using namespace enemies;


/**
 * The enemy class. The primary antagonists of the game.
 * A class which will randomly spawn and attempt to kill or
 * assist the main character.  Right now it's a fairly simple class,
 * but it should become increasingly complex as we go.
 */
class Enemy : public Character
{
    typedef std::vector<std::vector<Tile*> > TilePointerMatrix;
    protected:
        /**
         * The id of the enemy.
         * Each enemy type has an id, starting at 0, e.g. kobolds id=0.
         * This is to allow accessing things by index based on the id of the
         * enemy.
         */
        int id;

        /**
         * Member variable to hold how far the enemy can see.
         */
        int sight;

        /**
         * Member variable to hold where the enemy is looking.
         * This is 0-100, where 0 (and 100) are directly right.
         */
        int direction;

        /**
         * Member variable to hold the enemies field of view.
         * This is a percentage of the field of view (1-100).
         */
        int view;
        

        /**
         * Determines whether or not the enemy will flee.
         * If spooked is true, enemies will begin to flee from anything with a
         * morality that is different from its own.
         */
        bool spooked;

        /**
         * The direction that the spooked enemy will run in.
         * Once the enemy has been spooked, the direction for it to go will be
         * determined based on what spooked it.  direction_spooked will be an IntPoint
         * with the possible values of +/- 1 or 0.
         * @see get_spooked(IntPoint abs_coords, IntPoint target_abs)
         */
        IntPoint direction_spooked;

        /**
         * The number of this enemy's actions which have passed since it was spooked.
         */
        int time_spooked;

        /**
         * A string of the name of the enemy.
         * This is a generic string, as in "kobold," or "rabbit."
         */
        std::string name;

        /**
         * Generates the equipment from a list of EquipType.
         * Loops through a list of possible equipment that an enemy can have,
         * and the returned list goes into the inventory.
         * \todo Make the enemy equip the items.
         * @param equipment_list A list of possible equipment that the enemy can have.
         * @return A list of equipment that the enemy does have.
         */
        std::vector<Equipment*> generate_equipment(std::vector<EquipType> equipment_list);

        /**
         * Generates the weapon from a list of WeaponType.
         * Loops through a list of possible weapons that an enemy can have,
         * and the returned weapon goes into the inventory.
         * \todo Make the enemy equip the weapon.
         * \todo Posbbily make this multiple weapons and have it pick the best one.
         * @param weapon_list A list of possible weapons that the enemy can have.
         * @return The weapon that the enemy has.
         */
        Weapon* generate_weapon(std::vector<WeaponType>);

        /**
         * Changes the enemies x and y.
         * This function changes the enemies x and y.  It does not check
         * whether or not it is a valid move, that must be done by whatever
         * is calling this function.
         * @param x_change The change in the x direction.
         * @param y_change The change in the y direction.
         * @see x
         * @see y
         */
        void move(int x_change, int y_change);



        /**
         * Converts chunk/coords into the enemies surroundings coordinates.
         * Converts the passed in chunk/coordinates to absolute coordinates,
         * and then subtracts that from the top right of the enemy's line of
         * sight, givng coordinates relative to the enemy's LOS.
         * @param _chunk The chunk of the object to be converted.
         * @param _coords The coords of the object to be converted.
         * @param sur_chunk The chunk of top left corner of the surroundings.
         * @param sur_coords The coordinates of the top left corner of the surroundings.
         * @return The coordinates of the object where (0, 0) is the top corner of the enemies LOS>
         */
        IntPoint get_sur_coords(IntPoint sur_chunk, IntPoint sur_coords, IntPoint _chunk, IntPoint _coords);




        /**
         * Gets the direction that an enemy should be spooked.
         * Takes in a chunk and coordinates in the direction of the spooker,
         * and calculates the direction to run away from the the spooker.
         * @param abs_coords The absolute coordinates of the spook-e.
         * @param target_abs The absolute coordinates of the spooker.
         * @return The direction to run away in.
         * @see spooked
         */
        IntPoint get_spooked(IntPoint abs_coords, IntPoint target_abs);
       
        /**
         * Checks to see if a point is within the enemy's range of sight.
         * Looks at a point/chunk to see if it it's within the enemies
         * range of vision.  
         * @parameter _coords The coordinates of the target.
         * @parameter _chunk The chunk of the target.
         * @return True if the coords/chunk can be seen.
         */
        bool in_sight_range(IntPoint _coords, IntPoint _chunk);

        /**
         * Checks to see if the enemy can see something.
         * Looks at a point/chunk to see if it it's within the enemies
         * slice of view.  Assumes that the enemy can't see everything,
         * as having the upper and lower bounds being the same will make
         * everything pretty wonky.
         * @parameter _coords The coordinates of the target.
         * @parameter _chunk The chunk of the target.
         * @return True if the coords/chunk can be seen.
         */
        bool in_sight(IntPoint _coords, IntPoint _chunk);
        
        /**
         * Turns the enemy.
         * @parameter coords The difference between the target coords and current coords.
         * @todo Implement a max turn amount.
         */
        void turn(IntPoint coords);

        /**
         * Calculates the upper and lower angles (from 1-100) of the field of view.
         * @return An IntPoint containing the upper and lower bounds o the fov as angles.
         */
         IntPoint get_fov();
    public:
        /**
         * The default constructor.
         */
        Enemy();

        /**
         * Constructor without the chunk info.
         */
        Enemy(int _x, int _y, int _depth, EnemyType enemy);
        
        /**
         * The destructoid!
         */
        ~Enemy();

        /**
         * Public accessor for the sight.
         * @return The member variable sight.
         */
        int get_sight();

        /**
         * Public accessor for the id.
         * @return The member id.
         * @see id
         */
        int get_id();

        /**
         * A debugging function.
         * Dumps the paths to cout as they are calculated by a-star.  Only
         * enable if a-star is not working propery or if you like seeing the
         * a-star algrorithm work.  Or if you like massive dumps to cout.
         * Alternatively, you can use it to dump a different matrix.
         * @param map The matrix to dump.
         * @param tl The top left corner of the area to dump.
         * @param br The bottom right corner of the area to dump.
         */
        void dump_matrix(TilePointerMatrix& map, IntPoint tl, IntPoint br);

        /**
         * Gets all the coordinates that the enemy can currently see.
         * @return A list of coords the enemy can see.
         */
        std::vector<IntPoint> sight_tiles();
};


#endif
