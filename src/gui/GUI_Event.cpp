#include "GUI.h"

void GUI::OnEvent(SDL_Event* Event)
{
    VirtualEvent::OnEvent(Event);
}

void GUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    canvas.main_char.perform_action_press(sym);
}

void GUI::OnExit() 
{
    running = false;
}
