#ifndef _CANVAS_H
#define _CANVAS_H

#define STARTING_WIDTH 100
#define STARTING_HEIGHT 100

#include <SDL/SDL.h>
#include <stdlib.h>
#include <vector>

#include "DungeonBuilder.h"
#include "Dungeon.h"
#include "terrain_defs.h"
#include "Character.h"

using namespace std;
class Canvas 
{
    private:
        Dungeon dungeon;
        std::vector<std::vector<Tile> > canvas;
    
    public:
        Canvas();
        //TODO do something about these two. Events.cpp?
        DungeonBuilder db;
        Main_Character main_char;
        void refresh();
        void resize_canvas(int, int);
        const std::vector<std::vector<Tile> >& get_matrix();
        const Dungeon& get_dungeon();
};

#endif
