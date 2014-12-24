/**
 *  BEHAVIOR_NODE.CPP
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

#include <behavior_node.h>


SequenceNode(std::vector<BNode*> _nodes)
{
    nodes = _nodes;
}

int SequenceNode::tick(Actor* actor, Game* game)
{
    int did_succeed;
    for(int i=0;i<nodes.size();i++)
    {
        did_succeed = nodes[i]->tick(actor, game);
        if(did_succeed != SUCCESS)
        {
            return did_succeed;
        }
    }
    return SUCCESS;
}



PriorityNode::PriorityNode(std::vector<BNode*> _nodes)
{
    nodes = _nodes;
}

int PriorityNode::tick(Actor* actor, Game* game)
{
    int did_succeed;
    for(int i=0;i<nodes.size();i++)
    {
        did_succeed = nodes[i]->tick(actor, game);
        if(did_succeed != FAILURE)
        {
            return did_succeed;
        }
    }
    return FAILURE;
}

BranchingCondition::BranchingCondition(BNode* condition, BNode* success, BNode* failure)
{
    nodes.push_back(condition);
    nodes.push_back(success);
    nodes.push_back(failure);
}

int BranchingCondition::tick(Actor* actor, Game* game)
{
    int did_succeed = nodes[0]->tick(actor, game);
    if(did_succeed == SUCCESS)
    {
        return nodes[1]->tick(actor, game);
    }
    else if(did_succeed == FAILURE)
    {
        return nodes[2]->tick(actor, game);
    }
    else
    {
        return did_succeed;
    }
}


InverterNode::InverterNode(BNode* node)
{
    nodes.push_back(node);
}

int InverterNode::tick(Actor* actor, Game* game)
{
    int did_succeed = nodes[0]->tick(actor, game);
    if(did_succeed == FAILURE || did_succeed == SUCCESS)
    {
        return !did_succeed;
    }
    else
    {
        return did_succeed;
    }
}

int EnemyInRange::tick(Actor* actor, Game* game)
{
    return game->enemy_in_range(actor->get_character());
}

int LowHealth::tick(Actor* actor, Game* game)
{
    Character* chara = actor->get_character();
    return (float)chara->get_cur_hp() > (.2 * (float)chara->get_max_hp());
}

int MoveTowards::tick(Actor* actor, Game* game)
{
    Character* chara = actor->get_character();
    if(chara->get_target() == NULL)
    {
        return FAILURE;
    }
    IntPoint goal_coords = chara->get_target()->get_coords();
    IntPoint goal_chunk = chara->get_target()->get_chunk();
    return game->move_to_point(chara, goal_coords, goal_chunk);
}

int MoveAway::tick(Actor* actor, Game* game)
{

