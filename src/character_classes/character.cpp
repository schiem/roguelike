#include <character.h>
#include <iostream>

Character::Character()
{

}

/* PRE: Will be given a max health, a starting x and a starting y
 * The x and the y are the coordinates within the current chunk/dungeon
 * POST: A character object with the desired attributes will be returned
*/
Character::Character(int _max_health, int _x, int _y, int _sprite, Chunk const *_chunk, int _depth)
{
    current_health = _max_health;
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
    chunk = _chunk;
    depth = _depth;
}


void Character::display_character(int character, SDL_Surface* ascii, SDL_Surface* screen, Uint32 color){
    drawChr(x, y, character, ascii, screen, color); 
}

/* PRE: None
 * POST: Returns true if the character is still alive (health is > max_health)
*/
bool Character::is_alive() const{
    if (current_health <= 0){
        return false;
    }
    else{
        return true;
    }
}

/* PRE: Will be passed the desired change in x and y coordinates
 * POST: Will change the character's coordinates to match this
*/
void Character::move(int x_change, int y_change){
    std::cout<<"sprite:"<<chunk->get_tile(depth, y+y_change, x+x_change).sprite<<std::endl;
    if (chunk->get_tile(depth, y+y_change, x+x_change).can_be_moved_through){
        x += x_change;
        y += y_change;
}
}

/* PRE: Takes damage to reduce a character's health by
 * POST: Subtracts the damage from health and checks if the character is still alive
*/
void Character::take_damage(int damage){
    current_health -= damage;
    if (is_alive() == false){
        //delete the character, end the game
        }
}

int Character::get_x_loc(){
    return x;
}

int Character::get_y_loc(){
    return y;
}

int Character::get_char(){
    return sprite;
}

int Character::get_depth(){
    return depth;
}

int Character::get_chunk_x(){
    return chunk->get_x();
}

int Character::get_chunk_y(){
    return chunk->get_y();
}
