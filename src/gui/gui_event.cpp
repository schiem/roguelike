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
            canvas.main_char.perform_action_press(sym);
            break;
    }
}

void GUI::OnExit() {
    running = false;
}
