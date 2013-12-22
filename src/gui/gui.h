#ifndef _GUI_H
#define _GUI_H

#include <SDL/SDL.h>
#include <vector>

#include "chunk.h"
#include "virtual_event.h"
#include "canvas.h"
#include "terrain_defs.h"

class GUI : public VirtualEvent {

    typedef std::vector< std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        bool running;
        Canvas canvas;
        SDL_Event event;
        SDL_Surface* screen;
        SDL_Surface* asciiBase;
        SDL_Surface* ascii;

    public:
        GUI();
        int OnExecute();
        void OnRender();
        void OnLoop();
        void OnCleanup();
        bool OnInit();

        void OnEvent(SDL_Event* Event);
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnExit();

        void perform_action_press(SDLKey);
        void perform_action_cont();
        void char_move(int, int);
};

#endif
