#include "gui.h"

GUI::GUI() 
{
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;
    canvas = Canvas();

    running = true;
    cout<<"GUI construction complete."<<endl;
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
