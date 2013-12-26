#include <character.h>
#include <iostream>

Character::Character() {

}

/* PRE: Will be given a max health, a starting x and a starting y
 * The x and the y are the coordinates within the current chunk/dungeon
 * POST: A character object with the desired attributes will be returned
*/
Character::Character(int _max_health, int _x, int _y, Tile _sprite, int _depth) {
    current_health = _max_health;
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
    depth = _depth;
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
    if (is_alive() == false) {
        //delete the character
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

Tile Character::get_char() {
    return sprite;
}

int Character::get_depth() {
    return depth;
}
