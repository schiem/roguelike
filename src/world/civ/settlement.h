/**
 *  @file SETTLEMENT.H
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

#ifndef _SETTLEMENT_H
#define _SETTLEMENT_H

/**
 * The class to hold cities, towns and villages.
 * The metric for determing cities is as follows: 
 * Big Cities = 12,000-100,000 people, rarely exceeding 50,000.
 * Cities = 8,000-12,000 people.
 * Towns = 1,000-8,000 people.
 * Villages = 20-1,000 people.
 * Isolated dwellings = >20 people.
 * These will be generated in order of size to fill the population of a nation.  
 * The more rural settlements will make up the majority of the population 
 * (villages and isolated settlements), with ~3% of the population existing
 * in urbanized locations.  
 *
 * The layout of the settlement will also vary based on the size of the settlement,
 * the age of the settlement, and the age of the nation.  Older nations
 * (that have moved into feudalism) will tend to have a high number of
 * planned settlements (mostly towns), which means straight streets.  There
 * will also exist unplanned cities which are based around a single structure,
 * usually monastic or clerical, but occasionally secular.  
 *
 * The majority of towns, villages and isolated settlements will lie along the
 * trade routes between cities (the roads which join the cities). 
 * Villages and small settlements are more likely to be "off the beaten paht", 
 * while towns are more likely to exist along well travelled roads or useful landmarks
 * which may include rivers or mines.
 *
 * Towns and cities will likely be walled, depending on the level of threat
 *.  If there is a threat nearby, then it is likely that the city will be
 * contained within the walls.  It is also possible that the walls have
 * been expanded several times in order to accomodate growth.  This is 
 * largely dependent on the age of the settlement and the location.
 */

class Settlement
{
    private:
        /**
         * The number of people currently living in the settlement.
         */
        int population;

        /**
         * The number of houses in the settlement.
         * Calculated from the population.  Refers to any building where a 
         * person permantenly resides, even if it serves another purpose (e.g. has a
         * shop in the front).
         */
        std::vector<Home*> homes;

        /**
         * The number of buildings which are used for a purpose which is not housing.
         * This could be churches, secular office buildings, or any other
         * building which does not serve as someone's permaent residence.
         */
        std::vector<Building*> other_buildings;

        /**
         * Whether or not the settlement has walls.
         */
        bool walled;

        /**
         * The age of the settlement.
         */
        int age;

        /**
         * The number of stages of developments the town has undergone.
         * This will manifest as different sections of walls if the city
         * has walls, or a different pattern of building if it does not.
         * This is primarily determined by the age of the city.
         */
        int developments;

        /**
         * The number of centerpoints that the settlement has.
         * Occasionally, two towns would run into each other, or one town
         * would "split."  This would create binary towns, and theoretically
         * tertiary towns could exist.  This ends up looking like a town
         * in the process of mitosis with two very clear centers.
         */
        int num_focals;

        /**
         * The point(s) from which the settlement was built.
         * This is primarily for settlements which were not planned.
         * Planned settlements will most likely conform to a linear,
         * parallel street plan which does not necessarily have a center.
         * Centers may be buildings or landmarks, like rivers or defensible
         * hilltops.
         */
        std::vector<IntPoint> centers;

        /**
         * The governing figure of the town.
         * This figure may answer to someone higher, but that's not
         * necessary to know for the purpose of generation.  This may
         * also be NULL, which indicates that the town is self-governing.
         * The ruling figure helps to dictate the organization of the 
         * town as well as the presence of a larger structure like a 
         * castle or palace.  The larger the settlement, the less likely
         * it is that it will be self governing.
         * \todo Implement this, as NPCs don't exist yet.
         */
        //NPC* ruler;


        /**
         * The percentage of occupants of the settlement involved in agriculture.
         * This will effect the layout of the settlement.  Larger settlements
         * generally mean less agriculture.  The smallest settlements (villages
         * and isolated settlements) will be almost exculsively agricultural.
         */
         int agriculture;

        /** 
         * Whether or not the settlement is based in feudalism.
         * If the settlement is feudally based, then it is not truly
         * a town, village, or city.  Rather it has essentially become
         * a massive plantation where each tenant is given a plot of land
         * where a large percentage of the food is taken from them.  It 
         * will look like a series of farm plots with incredibly 
         * impoverished tenants.  Cities and towns were effectivley the
         * next step after feudalism, and as such the older the nation,
         * the fewer feudal settlements there will be.
         */
        bool feudal;

        /**
         * Whether or not the settlement was deliberately planned.
         * This will actually be the primary influence.  The older the
         * civilization, the more planned cities there will be.  These
         * will almost always have a ruler, and will typically have less
         * of a center. Planned towns will also have more parallel streets.
         * However, there will still be an element of chaos, more so the 
         * older the town is and the more developments which have been added.
         */
        bool planned;

        /**
         * The potential materials available to make buildings.
         * This represents the materials which can be used to create buildings.
         * This is subject to change over time.  It is determined by the technical
         * ability of the city and nation, and determines how high buildings can
         * be built.  Also has an influence on the rate of fires in a town.
         */
         std::vector<Tile> building_mats;

        /**
         * The potential materials to 
        
