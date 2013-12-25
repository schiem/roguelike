#include "gui.h"

GUI::GUI() : world_map(50,80) {
    current_screen = MAP_SCREEN;
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;
    //canvas = Canvas();

    running = true;
}

int GUI::OnExecute() {
    if(OnInit() == false) {
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
