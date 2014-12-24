/**
 *  BEHAVIOR_TREE.H
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

#ifndef _BEHAVIOR_TREE_H
#define _BEHAVIOR_TREE_H

#include <vector>
#include <behavior_node.h>
#include <behvarior_actor.h>

/**
 * Oh boy, let's get started on behavior trees.
 */
class BehaviorTree
{
    private:
        BehaviorNode* root;
        std::vector<BehaviorActors> actors;
