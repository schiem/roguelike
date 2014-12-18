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
 * A rather odd way of handling linked lists.
 * This is only really used in the A-star algorithm, and is designed to
 * act as a linked list by accessing on index instead of by memory address.
 * @see Enemy::a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
 */
struct ATile
{
    /**
     * The index value of the parent of this tile
     */
    int parent;

    /**
     * The coordinates of the tile.
     */
    IntPoint coords;

    /**
     * The total "score" for this tile.
     * Made up of f + g
     */
    int f;

    /**
     * The distance that has passed from the origin to this tile.
     */
    int g;

    /**
     * The result of the Manhattan Heuristic
     * @see Enemy::manhattan(IntPoint current, IntPoint goal)
     */
    int h;

    /**
     * Default constructor.
     */
    ATile()
    {
        f = -1;
        g = -1;
        h = -1;
    }

    /**
     * The constructor
     */
    ATile(int _parent, IntPoint _coords)
    {
        parent = _parent;
        coords = _coords;
        f = 0;
        g = 0;
        h = 0;
    }
};

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
         * Determines the best next move to make to reach a goal.
         * This function loods at the current coordinates, the coordinates of
         * the goal, and surroundings, and decides what the next move on the
         * shortest path is to reach that goal.
         * @param goal THe coordinates of the goal to reach
         * @param surroundings A matrix of the tiles surrounding the enemy.
         * @param cur_coords The current coordinates in the surroundings matrix.
         * @return The coordinates of the best next move.
         * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
         */
        IntPoint get_next_step(IntPoint goal, TilePointerMatrix& surroundings, IntPoint cur_coords);

        /**
         * Determines whether the coords are in the list of Tiles.
         * A helper function for the a_star which determines if a given
         * set of coords already exists in a list of ATiles.
         * @param point The point to check against.
         * @param list The list of ATiles which may possess the point.
         * @return The index of the location of the point in the list, or -1 if it is not in the list.
         * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix& surroundings)
         */
        int is_in(IntPoint point, std::vector<ATile> list);

        /**
         * A pathfinding algorithm.
         * This calculates the best path between two coordinates on a given array of tiles
         * using the a* algorithm.  This is done by keeping track of an "open" and a "closed"
         * list of tiles.  The open list represents tiles that could potentially be looked at,
         * and the closed list is a list of tiles that have already been looked at.
         *
         * The current tile is the tile that is currently being considered, and every tile around
         * it is added to the open list, assuming that those tiles can be moved through and are
         * not on the closed list.  Each tile added to the open list is given a g, h, and f score,
         * and the curren tile becomes the parent.
         * G represents the distance from the start point to the tile, h (standing for heuristic)
         * is a measure from the tile to the goal, and f = h + g.  If a tile is on the open list,
         * then f is recalculated for the current tile.  If it is lower, then the current tile becomes
         * the new parent.
         *
         * Each loop, the current tile is the one on the open list with the shortest f score.  If the
         * goal is on the open list, or the open list is empty, then the algorithm is stopped.
         * See the source for further documentation.
         * @param start The starting point for the algorithm.
         * @param goal The place the enemy is trying to get to.
         * @param surroundings The surroundings within the sight of the enemy.
         * @return A vector containing a list of IntPoints representing the best path, or an empty vector if there is no path.
         */
        std::vector<IntPoint> a_star(IntPoint start, IntPoint goal, TilePointerMatrix &surroundings);

        /**
         * A heuristic to estimate the distance from a point to the goal.
         * A helper function for the a-star algorithm, it calculates the
         * distance as the x_change + y_change between a point and the goal.
         * @param current The current point.
         * @param goal The goal the enemy is trying to reach.
         * @return The evaluated value.
         * @see a_star(IntPoint start, IntPoint goal, TilePointerMatrix&surroundings)
         */
        int manhattan(IntPoint, IntPoint);

        /**
         * Returns the ATile with the smallest f value.
         * A helper fucntion for the a-star algorithm which finds the smallest
         * f-value.
         * @param A reference to a list of ATiles.
         * @return The index of the smallest f value found in the list.
         */
        int get_smallest_f(std::vector<ATile>& list);

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
         * Finds a target for non-passive enemies.
         * Finds the best target to act on by searching for the character
         * with the closest morality to the target_id.
         * @param target_id The target morality that the enemy is looking for.
         * @param selectability The range that the morality can fall within.
         * @param enemy_list The list of enemies to check.
         * @return The character that is found as the best target.  NULL if none is found.
         */
        Character* find_best_target(int target_id, int selectability, std::vector<Character*> enemy_list);

        /**
         * Finds an enemy that isn't pasive.
         * If a character is found that isn't passive, it returns that
         * character.  Otherwise it returns NULL.
         * @param selectability The range that the morality can fall within.
         * @param enemy_list The list of enemies to check.
         * @return The character that is found as the best target.  NULL if none is found.
         */
        Character* passive_best_target(int target_id, int selectability, std::vector<Character*> enemy_list);


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
         * Wrapper function for calling different types of AI.
         * Depending on the type of enemy, this function will call a different
         * type of AI.
         * It is necessary to pass in the sur_chunk and sur_coords so that
         * the coordinates of the character an be converted into the coords
         * of the surroundnigs tilematrix.
         * @param surroundings The tiles around the enemy.
         * @param sur_chunk The chunk of the surrounding coords.
         * @param sur_coords The coordinates of the surrounding tilematrix.
         * @param char_list The list of characters the enemy can see.
         * @param delta_ms The change in millisenconds since the last time ai was called.
         * @see aggressive_ai(TilePointerMatrix surroundings, std::vector<character*> char_list, long delta_ms)
         * @see passive_ai(TilePointerMatrix surroundings, std::vector<character*> char_list, long delta_ms)
         */
        void run_ai(TilePointerMatrix &surroundings, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms);

        /**
         * Ensures that the target is still within the view.
         */
        bool validate_target();
        
        
        /**
         * The ai for the aggressive enemies.
         * The AI searches for a target (find_best_target()) and moves towards
         * that target.  If it is next to the target, it will attack it.
         * If no target is found, it will move randomly.
         * @param surroundings The tiles around the enemy.
         * @param sur_chunk The chunk of the surrounding coords.
         * @param sur_coords The coordinates of the surrounding tilematrix.
         * @param char_list The list of characters the enemy can see.
         * @param delta_ms The change in millisenconds since the last time ai was called.
         */
        void aggressive_ai(TilePointerMatrix&, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms);

        /**
         * The ai for the passive enemies.
         * The AI searches for a target (passive_best_target())
         * and will run away from any non-passive targets found.  As time
         * passes, it has a greater and greater chance of being un-spooked
         * and going back to grazing.
         * @param surroundings The tiles around the enemy.
         * @param sur_chunk The chunk of the surrounding coords.
         * @param sur_coords The coordinates of the surrounding tilematrix.
         * @param char_list The list of characters the enemy can see.
         * @param delta_ms The change in millisenconds since the last time ai was called.
         * \todo Make them find a food source and move toward that.
         * @see spooked
         */
        void passive_ai(TilePointerMatrix &surroundings, IntPoint sur_chunk, IntPoint sur_coords, std::vector<Character*> char_list, long delta_ms);

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
