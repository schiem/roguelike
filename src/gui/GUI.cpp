#include "GUI.h"

GUI::GUI() 
{
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;

    running = true;
}

int GUI::OnExecute()
{   
    if(OnInit() == false)
    {
        return -1;
    }

    SDL_Event e;

    while(running) {
        while(SDL_PollEvent(&e)) {
            OnEvent(&e);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return 0;
}
