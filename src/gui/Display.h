#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <SDL/SDL.h>
#include <ASCII_Lib.h>
#include <stdio.h>
#include <iostream>
#include <def.h>
#include <stdlib.h>
#include <vector>

#include "Canvas.h"
#include "terrain_defs.h"


class Display
{
    typedef std::vector<std::vector<Tile> > TileMatrix;
    private:
        SDL_Event event;
        SDL_Surface* screen;
        SDL_Surface* asciiBase;
        SDL_Surface* ascii;
        Canvas canvas;


    public:
        Display();
        //TODO TODO TODO Don't have this here lololol
        bool get_input();
        void refresh();
        void cleanup();
};

#endif
