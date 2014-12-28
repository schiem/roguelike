/**
 *  @file AI_DEFS.CPP
 *  @author Seth Yoder, Michael Yoder
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

#include <ai_defs.h>

//Make one of these for EVERY TREE YOU MAKE.
////////////////////////GENERIC AGGRESSIVE AI//////////////////////////
//  
//                * = Priority                                      
//                > = Sequence                                      
//                } = Branching Conditional                         
//                                                                  
//                                                                  
//                                  Root*                           
//                                   +                              
//                                   |                              
//         +----------------------------------------------------+   
//         |                         |                          |   
//         v                         v                          v   
//       Death>                   Attack>                    Wander 
//         +                         +                              
//         |                         |                              
//     +---+--+             +--------+--------+                     
//     |      |             |                 |                     
//     v      v             v                 v                     
// Inverter  Die     EnemyInRange             }                     
//     +                                      +                     
//     |                                      |                     
//     v                            +------------------+            
// HasHealth                        |         |        |            
//                                  v         v        v            
//                              LowHealth  MoveAway    }            
//                                                     +            
//                                                     |            
//                                            +----------------+    
//                                            |        |       |    
//                                            v        v       v    
//                                         NextTo   Attack  MoveTowards
//
///////////////////////////////////////////////////////////////////////

BehaviorTree ai::GENERIC_AGGRESSIVE(Game* game)
{
    BranchingCondition* first = new BranchingCondition(new NextTo, new Attack, new MoveTowards);
    BranchingCondition* second = new BranchingCondition(new LowHealth, new MoveAway, first);
    std::vector<BNode*> attack_nodes;
    attack_nodes.push_back(new EnemyInRange);
    attack_nodes.push_back(second);
    SequenceNode* attack = new SequenceNode(attack_nodes);

    std::vector<BNode*> death_nodes;
    death_nodes.push_back(new InverterNode(new HasHealth));
    death_nodes.push_back(new Die);
    SequenceNode* death = new SequenceNode(death_nodes);

    std::vector<BNode*> root;
    root.push_back(death);
    root.push_back(attack);
    root.push_back(new Wander);
    PriorityNode* root_node = new PriorityNode(root);   
    
 
    BehaviorTree tree = BehaviorTree(root_node, game, 0);
    return tree;
}
