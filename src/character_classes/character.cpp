/**
 *  CHARACTER.CPP
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

#include <character.h>

Character::Character() {

}

/* PRE: Will be given a max health, a starting x and a starting y
 * The x and the y are the coordinates within the current chunk/dungeon
 * POST: A character object with the desired attributes will be returned
*/
Character::Character(int _max_health, int _x, int _y, Tile _sprite, Tile _corpse, int _chunk_x, int _chunk_y,  int _depth, int _morality, int _attack) {
    current_health = _max_health;
    max_health = _max_health;
    x = _x;
    y = _y;
    moral = _morality;
    corpse = _corpse;
    //somewhat temporary
    attack_dam= _attack;
    armor = 0;
    
    sprite = _sprite;
    cout<<_chunk_y<<", "<<_chunk_x<<endl;
    chunk = IntPoint(_chunk_y, _chunk_x);
    depth = _depth;
    target = NULL;
}

/*
 * Constructor for the character class
 */
Character::Character(int _x, int _y, int _chunk_x, int _chunk_y, int _depth)
{
    x = _x;
    y = _y;
    chunk = IntPoint(_chunk_y, _chunk_x);
    depth = _depth;
    target = NULL;
}

/* PRE: None
 * POST: Returns true if the character is still alive (health is > max_health)
*/
bool Character::is_alive() const {
    if (current_health <= 0){
        return false;
    } else {
        return true;
    }
}

/* PRE: Will be passed the desired change in x and y coordinates
 * POST: Will change the character's coordinates to match this
*/

/* PRE: Takes damage to reduce a character's health by
 * POST: Subtracts the damage from health and checks if the character is still alive
*/
void Character::take_damage(int damage){
    current_health -= damage;
}

void Character::attack(Character* _chara)
{
    _chara->take_damage(attack_dam);
}

vector<Item*> Character::get_inventory()
{
    return inventory;
}

vector<Item*> Character::get_equipment()
{
    return equipment;
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

void Character::set_armor(int a)
{
    armor = a;
}

int Character::get_x() {
    return x;
}

int Character::get_y() {
    return y;
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

Tile Character::get_corpse(){
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
    return max_health;
}

int Character::get_cur_hp()
{
    return current_health;
}

int Character::get_moral()
{
    return moral;
}

int Character::get_armor()
{
    return armor;
}

