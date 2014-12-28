/**
 *  BEHAVIOR_NODE.H
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

#ifndef _BEHAVIOR_NODE_H
#define _BEHAVIOR_NODE_H

#include <vector>
#include <game.h>
#include <behavior_actor.h>

class BActor;

enum NODE_STATES
{
    FAILURE,
    SUCCESS,
    RUNNING,
    DEAD
};

/**
 * Defines the different nodes for a behavior tree.
 */
class BNode
{
    protected:
        /**
         * The children nodes of the object.
         */
        std::vector<BNode*> nodes;
    public:
        
        /**
         * Ticks the node forward once.
         */
        virtual int tick(BActor* actor, Game* game)=0;
};

//-------------------------COMPOSITE NODES------------------//

/**
 * A composite node responsible for running nodes in a sequence.
 * It will call the child nodes until one of them returns false.
 */
class SequenceNode : public BNode
{
    public:
        SequenceNode(std::vector<BNode*> _nodes);
        int tick(BActor* actor, Game* game);
};

/**
 * A composite nodes responsible for running nodes in order until
 * one returns true.
 */
class PriorityNode : public BNode
{
    public:
        PriorityNode(std::vector<BNode*> nodes);
        int tick(BActor* actor, Game* game);
};

/**
 * A class to emulate if/else statements, which is dangerous territory
 * as it could become overrused.  The problem now, however, is that if
 * you want a condition to execute one of two behaviors depending on 
 * success or failure, you have to put a priority with the two behaviors
 * you want, and then put the condition at the beginning of the first node,
 * so that if it fails you move to the second.  However, if the node
 * were to fail for another reason (like, for instance, a character
 * being unable to move), then you would move onto the second one anyway.
 * This is a problem, so I made a branching conditional.  It will execute
 * the first node if the condition returns success, otherwise it will
 * execute the second.
 */
class BranchingCondition : public BNode
{
    public:
        BranchingCondition(BNode* condition, BNode* success, BNode* failure);
        int tick(BActor* actor, Game* game);
};

//---------------------------DECORATOR NODES---------------------//
/**
 * Inverts whatever is returned from the child node.
 */
class InverterNode : public BNode
{
    public:
        InverterNode(BNode* node);
        int tick(BActor* actor, Game* game);
};


//-------------------------------ACTION NODES---------------------//

class MoveTowards : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};

class MoveAway : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};

class Attack : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};
        
class Wander : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};


class Die : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};
//------------------------------CONDITION NODES---------------------//

class LowHealth : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};

class EnemyInRange : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};

class NextTo : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};

class HasHealth : public BNode
{
    public:
        int tick(BActor* actor, Game* game);
};



#endif
