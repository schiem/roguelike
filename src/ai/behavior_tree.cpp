/**
 *  BEHAVIOR_TREE.CPP
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

#include <behavior_tree.h>

BehaviorTree::BehaviorTree(BNode* node, Game* _game)
{
    root = node;
    game = _game;
}

BehaviorTree::~BehaviorTree()
{
    for(int i=0;i<actors.size();i++)
    {
        delete actors[i];
    }
}

void BehaviorTree::add_actor(BActor* actor)
{
    actors.push_back(actor);
}

void BehaviorTree::run_actors(long delta_ms)
{
    for(int i=0;i<actors.size();i++)
    {
        BActor* actor = actors[i];
        if(actor->should_tick(delta_ms))
        {
            int status = root->tick(actor, game);
            if(status == DEAD)
            {
                if(!game->character_in_range(actor->get_character()))
                {
                    game->remove_enemy(actor->get_character());
                }
                else
                {
                    game->kill(actor->get_character());
                }
                actors.erase(actors.begin() + i);
                delete actor;
            }
        }
    }
}

void BehaviorTree::remove_actor(Character* chara)
{
    for(int i=0;i<actors.size();i++)
    {
        if(actors[i]->get_character() == chara)
        {
            actors.erase(actors.begin() + i);
        }
    }
}
