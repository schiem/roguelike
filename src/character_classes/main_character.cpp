#include <character.h>
#include <iostream>

Main_Character::Main_Character() {

}

Main_Character::Main_Character(int _max_health, int _x, int _y,
                               int _sprite,  int _depth) :
    Character(_max_health, _x, _y,  _sprite, _depth){

}
