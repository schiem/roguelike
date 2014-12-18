/**
 *  @file CHARACTER.CPP
 *  @author Michael Yoder
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

#include <character.h>

Character::Character() {

}

Character::Character(int _x, int _y, int _depth)
{
    stats.resize(3);
    x = _x;
    y = _y;
    chunk = IntPoint(0, 0);
    depth = _depth;
    target = NULL;
    equipment = vector<Item*>(7);
    conscious = true;
}

Character::Character(std::vector<int> _stats, int _x, int _y, Tile _sprite, MiscType _corpse, int _chunk_x, int _chunk_y, int _depth, int _morality, int _speed) {

    timer = 0;
    stats = _stats;
    current_stats = stats;
    x = _x;
    y = _y;
    moral = _morality;
    corpse = new Misc(IntPoint(y, x), _corpse);
    //somewhat temporary
    sprite = _sprite;
    chunk = IntPoint(_chunk_y, _chunk_x);
    depth = _depth;
    speed = _speed;
    target = NULL;
    equipment = vector<Item*>(7);
    conscious = true;
}


void Character::act(long ms)
{
    timer += ms;
    if(timer >= speed)
    {
        timer -= speed;
        gain_endurance(1);
        if(!is_conscious())
        {
            if(current_stats[ENDURANCE] > stats[ENDURANCE]/current_stats[ENDURANCE])
            {
                regain_consciousness();
            }
        }
    }
}

bool Character::is_alive() const {
    if (current_stats[HEALTH] <= 0){
        return false;
    } else {
        return true;
    }
}

void Character::take_damage(int damage){
    current_stats[HEALTH] -= damage;
}

void Character::attack(Character* _chara)
{
    _chara->take_damage(current_stats[ATTACK]);
    _chara->set_target(this);
}




vector<Item*>* Character::get_inventory()
{
    return &inventory;
}

vector<Item*>* Character::get_equipment()
{
    return &equipment;
}

void Character::add_item(Item* new_item)
{
    inventory.push_back(new_item);
}

void Character::drop_item(Item* item)
{
    for(int i=0;i<inventory.size();i++)
    {
        if(inventory[i] == item)
        {
            inventory.erase(inventory.begin() + i);
        }
    }
}

void Character::drop_item(int item)
{
    if(item < inventory.size())
    {
        inventory.erase(inventory.begin() + item);
    }
}

void Character::destroy_item(Item* item)
{
    for (int i=0; i<inventory.size();i++)
    {
        if(inventory[i] == item)
        {
            delete inventory[i];
            inventory.erase(inventory.begin() + i);
        }
    }
}

void Character::equip_item(Item* item)
{
    if(item->can_equip)
    {
        drop_item(item);
        remove_item(((Equipment*)item)->get_body_part());
        equipment[((Equipment*)item)->get_body_part()] = item;
    }
    else if(item->can_wield)
    {
        drop_item(item);
        remove_item(6);
        equipment[6] = item;
        current_stats[ATTACK] = ((Weapon*)item)->get_damage();
    }
}

void Character::remove_item(int item)
{
    if(equipment[item] != NULL)
    {
        if(item < 6)
        {
            add_item(equipment[item]);
            equipment[item] = NULL;
        }
        else
        {
            current_stats[ATTACK] = stats[ATTACK];
            add_item(equipment[item]);
            equipment[item] = NULL;
        }
    }
}

void Character::remove_all()
{
    for(int i=0;i<equipment.size();i++)
    {
        remove_item(i);
    }
}


void Character::set_x(int _x) {
    x = _x;
}

void Character::set_y(int _y) {
    y = _y;
}

void Character::set_depth(int d) {
    depth = d;
}

int Character::get_x() {
    return x;
}

int Character::get_y() {
    return y;
}

IntPoint Character::get_coords(){
    return IntPoint(y, x);
}

int Character::get_chunk_x()
{
    return chunk.col;
}

int Character::get_chunk_y()
{
    return chunk.row;
}

IntPoint Character::get_chunk()
{
    return chunk;
}

void Character::set_chunk(IntPoint _chunk)
{
    chunk = _chunk;
}

void Character::set_chunk_x(int _chunk_x)
{
    chunk.col = _chunk_x;
}

void Character::set_chunk_y(int _chunk_y)
{
    chunk.row = _chunk_y;
}


Tile Character::get_char() {
    return sprite;
}

Item* Character::get_corpse(){
    return corpse;
}

int Character::get_depth() {
    return depth;
}

void Character::set_target(Character* _target)
{
    target = _target;
}

Character* Character::get_target()
{
    return target;
}

int Character::get_max_hp()
{
    return stats[HEALTH];
}

int Character::get_cur_hp()
{
    return current_stats[HEALTH];
}

int Character::get_moral()
{
    return moral;
}

int Character::get_armor_hit(int body_part, int type)
{
    Item* item = equipment[body_part];
    if(item != NULL)
    {
        return ((Equipment*)item)->get_hit(type);
    }
    else{
        return 0;
    }
}

float Character::get_armor_dam(int body_part, int type)
{
    Item* item = equipment[body_part];
    if(item != NULL)
    {
        return ((Equipment*)item)->get_dam(type);
    }
    else{
        return 0;
    }
}

void Character::consume_item(Item* item)
{
    if(item->can_consume)
    {
        int type = ((Consumable*)item)->get_type();
        int stat = ((Consumable*)item)->get_stat();
        int amount = ((Consumable*)item)->get_amount();
        if(type == consumables::INCREASE)
        {
            stats[stat] += amount;
        }
        else
        {
            current_stats[stat] += amount;
            if(((Consumable*)item)->get_type() == consumables::RESTORE)
            {
                if(current_stats[stat] > stats[stat])
                {
                    current_stats[stat] = stats[stat];
                }
            }
        }
        destroy_item(item);
    }
}

int Character::get_stat(int stat)
{
    return stats[stat];
}

void Character::set_stat(int stat, int amount)
{
    stats[stat] = amount;
}

int Character::get_current_stat(int stat)
{
    return current_stats[stat];
}

void Character::set_current_stat(int stat, int amount)
{
    current_stats[stat] = amount;
}

void Character::regain_consciousness()
{
    conscious = true;
}

void Character::pass_out()
{
    conscious = false;
}

bool Character::is_conscious()
{
    return conscious;
}

void Character::reduce_endurance(int factor)
{
    int base = 10;
    int reduction = factor * (base/get_current_stat(STRENGTH));
    current_stats[ENDURANCE] -= reduction;
    if(get_current_stat(ENDURANCE) <= 0)
    {
        set_current_stat(ENDURANCE, 0);
        pass_out();
    }
}

void Character::gain_endurance(int factor)
{
    int base = 10;
    int gain = factor * (base/get_current_stat(STRENGTH));
    current_stats[ENDURANCE] += gain;
    if(get_current_stat(ENDURANCE) > stats[ENDURANCE])
    {
        set_current_stat(ENDURANCE, stats[ENDURANCE]);
    }
}

bool Character::can_act()
{
    return is_conscious();
}
