/**
 *  ENEMY_DEFS.H
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

#include <enemy_defs.h>

using namespace tiledef;
using namespace equipment;
using namespace weapons;
using namespace boost::assign;

namespace enemies
{
    EquipType kob_eq[] = {boots};
    WeaponType kob_wp[] = {dagger};
    EnemyType kobold = {5, 20, 2, 0, 1, 20, 100, "Kobold",  KOBOLD, KOBOLD_CORPSE, std::vector<EquipType>(&kob_eq[0], &kob_eq[0]+1), std::vector<WeaponType>(&kob_wp[0], &kob_wp[0]+1)}; 
    EnemyType rabbit = {3, 10, 0, 0, 2, 15, (rand() % (20 + 50)), "Rabbit",  RABBIT, RABBIT_CORPSE, std::vector<EquipType>(), std::vector<WeaponType>()}; 
}
