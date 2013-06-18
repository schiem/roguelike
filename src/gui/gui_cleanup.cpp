#include "gui.h"

void GUI::OnCleanup()
{
    SDL_FreeSurface (ascii);
    SDL_Quit();
}
