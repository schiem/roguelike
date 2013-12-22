#include "gui.h"

void GUI::OnEvent(SDL_Event* Event) {
    VirtualEvent::OnEvent(Event);
}

void GUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_q:
            running=false;
            break;

        default:
            perform_action_press(sym);
            break;
    }
}

void GUI::OnExit() {
    running = false;
}


/**
 * There is possibly a better place for these functions, but they used to be in
 * MainCharacter, and it seems weird for the main character to know about SDL.
 * I'm moving more to a top-down structure, where the GUI classes control the
 * canvas and key handling, which in turn control the main character.
 *
 * On the flip side, it seems weird for the GUI to know about the main
 * character, but I would err on the side of top-down instead of bottom-up
 * control. Perhaps in the future there will be an intermediate class. Actually,
 * there really should be.
 *
 * -SAY 12/21/2013
 */

/**
 * The problem that I'm finding with this model is that we don't have
 * any functionality in the character...all of the processing has been
 * handed to the GUI classes.  What I'm thinking should happen is the
 * GUI calls functions in the character with the appropriate values
 * being passed.  That way, the gui isn't doing processing that should
 * really belong to the character.  But, then the problem with that is
 * character doesn't know anything about the chunk...which leads me to
 * think that the GUI should pass functions to the canvas, which will
 * handle the processing by calling appropriate functions in the classes.
 * -MJY 12/22/2013...holy balls, Christmas is 3 days away...
 */



void GUI::perform_action_press(SDLKey key) {
    //This is a pointer to a const value. The pointer can be modified, but the
    //value at the pointer cannot be modified from this name. Future Seth:
    //remember that 'const' is left-binding.
    switch (key) {
        case SDLK_u:
            canvas.change_main_depth(-1);
        case SDLK_d:
            canvas.change_main_depth(1);
        default:
            break;
    }
}


void GUI::perform_action_cont() {
    SDL_PumpEvents();
    Uint8* keystate = SDL_GetKeyState(NULL);

    if(keystate[SDLK_LEFT]){
        canvas.move_main_char(-1, 0);
    }
    if(keystate[SDLK_RIGHT]){
        canvas.move_main_char(1, 0);
    }
    if(keystate[SDLK_UP]){
        canvas.move_main_char(0, -1);
    }
    if(keystate[SDLK_DOWN]){
        canvas.move_main_char(0, 1);
    }
}

