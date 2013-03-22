#include <Character.h>
#include <ncurses.h>

Main_Character::Main_Character(int _max_health, int _x, int _y, char _sprite) : Character(_max_health, _x, _y,  _sprite){
    max_health = _max_health;
    x = _x;
    y = _y;
    sprite = _sprite;
}

void Main_Character::perform_action(char keypress){
    switch(keypress){
        case (char)KEY_LEFT:
            move(-1, 0);
            break;
        case (char)KEY_RIGHT:
            move(1, 0);
            break;
        case (char)KEY_UP:
            move(0, -1);
            break;
        case (char)KEY_DOWN:
            move(0, 1);
            break;
        default:
            break;
    }
}
