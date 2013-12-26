#ifndef _GUI_H
#define _GUI_H

#include <SDL/SDL.h>
#include <vector>

#include "chunk.h"
#include "virtual_event.h"
#include "game.h"
#include "terrain_defs.h"
#include "world_map_gui.h"
#include "def.h"

class GUI : public VirtualEvent {

    typedef std::vector< std::vector<Tile*> > TilePointerMatrix;
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        //This is by no means a good system. Just temporary.
        static int const MAP_SCREEN = 0;
        static int const GAME_SCREEN = 1;

        bool running;
        int current_screen;

        WorldMapGUI world_map_gui;
        Game canvas;
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
};

#endif
