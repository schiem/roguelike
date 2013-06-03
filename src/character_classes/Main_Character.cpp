#include <Character.h>

Main_Character::Main_Character(int _max_health, int _x, int _y, int _sprite) : Character(_max_health, _x, _y,  _sprite){
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
}


void Main_Character::perform_action(Uint8 keypress){
	if(keypress[SDLK_LEFT]){
		move(-1, 0);
	}
	if(keypress[SDLK_RIGHT]){
		move(1, 0);
	}
	if(keypress[SDLK_UP]){
		move(0, -1);
		}
	if(keypress[SDLK_DOWN]){
		move(0, 1);
	}
}
