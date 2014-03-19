/**
 *  GUI.CPP
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gui.h>

//this is for backward compatibility with c98 standard
std::string temp_array[2] = {"Start a New Game", "Continue"};
std::vector<std::string> menu_selection (&temp_array[0], &temp_array[0]+2); 

GUI::GUI() : world_map_gui(100, 50),
             menu(1, BLOCK_WALL, menu_selection, "Some Name"),
             game(100, 50) {

    current_screen = MENU_SCREEN;
    screen = NULL;
    asciiBase = NULL;
    ascii = NULL;

    running = true;
}

int GUI::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event e;

    while(running) {
        game_clock = pt::microsec_clock::local_time();
        while(SDL_PollEvent(&e)) {
            OnEvent(&e);
        }

        OnLoop();
        OnRender();

        SDL_Delay(handle_framerate());

    }

    OnCleanup();


    return 0;
}

//TODO IN CASE THE USER'S COMPUTER IS SLOW, game.act should receive the actual
//frame time instead of the intended frame time. Then it will be true frame
//slowing. As of right now (1/6/2014) this is very unlikely to be an issue, as
//my 10 year old laptop can handle the game twice as fast. However, we don't
//know what it will be like in the future.
int GUI::handle_framerate() {
    pt::ptime clock2 = pt::microsec_clock::local_time();
    long this_frame_time = (clock2 - game_clock).total_milliseconds();
    //cout<<STD_MS_PER_FRAME - this_frame_time<<endl;
    if(this_frame_time < STD_MS_PER_FRAME) {
        return STD_MS_PER_FRAME - this_frame_time;
    } else {
        //No matter what, we want to delay at least a little bit.
        return 10;
    }
}
