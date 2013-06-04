#include <Character.h>
#include <iostream>

Main_Character::Main_Character(int _max_health, int _x, int _y, int _sprite ) : Character(_max_health, _x, _y,  _sprite){
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
	current_dungeon;
}
/*
Main_Character::~Main_Character(){
	delete current_dungeon;
}
*/
void Main_Character::update_dungeon(Dungeon dungeon){
	current_dungeon = dungeon;
}


void Main_Character::perform_action_cont(){
	SDL_PumpEvents();
	Uint8* keystate = SDL_GetKeyState(NULL);
	
	if(keystate[SDLK_LEFT]){
		if (current_dungeon.get_tile(x-1, y).can_be_moved_through){
			move(-1, 0);
		}
	}
	if(keystate[SDLK_RIGHT]){
		if (current_dungeon.get_tile(x+1, y).can_be_moved_through){
			move(1, 0);
		}
	}
	if(keystate[SDLK_UP]){
		if (current_dungeon.get_tile(x, y-1).can_be_moved_through){
				move(0, -1);
			}
		}
	if(keystate[SDLK_DOWN]){
		if (current_dungeon.get_tile(x, y+1).can_be_moved_through){
			move(0, 1);
		}
	}
}

void Main_Character::perform_action_press(SDLKey key){
	switch (key)
	{
		case SDLK_ESCAPE:
			std::cout<<"I'm escaping now, but not continuously!"<<std::endl;
			break;
		default:
			break;
	}
	
}
