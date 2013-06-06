#include "GUI.h"

void GUI::OnCleanup()
{
	SDL_FreeSurface (ascii);
	SDL_Quit();
}
