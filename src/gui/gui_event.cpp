/**
 *  @file GUI_EVENT.CPP
 *  @author Michael & Seth Yoder
 *
 *  @section LICENSE
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

/**
 * \todo find a better way to do key input; currently we're using polling, which
 * feels really weird if you press a key in-between frames.
 */
void GUI::OnEvent(SDL_Event* Event) {
    VirtualEvent::OnEvent(Event);
}

void GUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
    switch(sym) {
        case SDLK_q:
            running=false;
            break;

        default:
            perform_action_press(sym);
            break;
    }
}

void GUI::OnExit() {
    running = false;
}


/**
 * There is possibly a better place for these functions, but they used to be in
 * MainCharacter, and it seems weird for the main character to know about SDL.
 * I'm moving more to a top-down structure, where the GUI classes control the
 * canvas and key handling, which in turn control the main character.
 *
 * On the flip side, it seems weird for the GUI to know about the main
 * character, but I would err on the side of top-down instead of bottom-up
 * control. Perhaps in the future there will be an intermediate class. Actually,
 * there really should be.
 *
 * -SAY 12/21/2013
 */

/**
 * The problem that I'm finding with this model is that we don't have
 * any functionality in the character...all of the processing has been
 * handed to the GUI classes.  What I'm thinking should happen is the
 * GUI calls functions in the character with the appropriate values
 * being passed.  That way, the gui isn't doing processing that should
 * really belong to the character.  But, then the problem with that is
 * character doesn't know anything about the chunk...which leads me to
 * think that the GUI should pass functions to the canvas, which will
 * handle the processing by calling appropriate functions in the classes.
 * -MJY 12/22/2013...holy balls, Christmas is 3 days away...
 */



void GUI::perform_action_press(SDLKey key) {
    //This is a pointer to a const value. The pointer can be modified, but the
    //value at the pointer cannot be modified from this name. Future Seth:
    //remember that 'const' is left-binding.
    switch (key) {
        case SDLK_RETURN:
            if(current_screen == MENU_SCREEN) {

                //get the next menu from the selection
                menu = menu->make_selection();
                //if the menu returned NULL, then we should quit the game
                if(menu == NULL)
                {
                    running = false;
                    menu = new EscapeMenu(1, BLOCK_WALL, &game);
                    current_screen = menu->get_screen();
                }
                
                //if the menu is the font menu, then we should change the font
                if(menu->get_id() == 5)
                {
                    //only change the font if a font has been selected (i.e. they didn't
                    //push back
                    if(((FontMenu*)menu)->get_font() != "")
                    {
                        load_font(((FontMenu*)menu)->get_font());
                    }
                }

                //switch back to the game and unpause
                if(menu->should_exit()) 
                {
                    current_screen = menu->get_screen();
                    game.unpause();
                }
            } else if(current_screen == MAP_SCREEN) {
                //switch the screen from the map to the game (should only happen at 
                //the beginnig of the game
                current_screen = GAME_SCREEN;
            } 

            //render the canvas to clear away any old menus that might be lingering
            render_canvas();
            break;
        case SDLK_ESCAPE:
            //open up the escape menu
            if(current_screen == GAME_SCREEN) 
            {
                //switch the menu to the escape menu and 
                //delete the old one
                current_screen = MENU_SCREEN;
                delete menu;
                menu = new EscapeMenu(1, BLOCK_WALL, &game);
                game.pause();
            }
            else if (current_screen == MENU_SCREEN)
            {
                current_screen = menu->get_screen();
                game.unpause();
            }
            break;
        case SDLK_u:
            if(!game.is_paused() && current_screen == GAME_SCREEN)
            {
                game.change_main_depth(-1);
            }
            break;
        case SDLK_d:
            if(!game.is_paused() && current_screen == GAME_SCREEN)
            {
                game.change_main_depth(1);
            }
            break;
        case SDLK_m:
            if(current_screen == GAME_SCREEN)
            {
                current_screen = MENU_SCREEN;
                delete menu;
                menu = new MainMenu(1, BLOCK_WALL, &game);
                game.pause();
            }
            break;
        case SDLK_g:
            if(!game.is_paused() && current_screen == GAME_SCREEN)
            {
                game.get_item();
            }
            break;
        case SDLK_SPACE:
            if(current_screen == GAME_SCREEN)
            {
                game.toggle_pause();
            }
            break;
        default:
            break;
    }
}


void GUI::perform_action_cont() {
    SDL_PumpEvents();
    Uint8* keystate = SDL_GetKeyState(NULL);

    if (current_screen == MENU_SCREEN) {
        if(keystate[SDLK_UP]) {
            menu->move_selection(-1);
        }
        if(keystate[SDLK_DOWN]) {
            menu->move_selection(1);
        }
    } else if (current_screen == MAP_SCREEN) {
        if(keystate[SDLK_LEFT]){
            world_map_gui.move_cursor(-1, 0);
        }
        if(keystate[SDLK_RIGHT]){
            world_map_gui.move_cursor(1, 0);
        }
        if(keystate[SDLK_UP]){
            world_map_gui.move_cursor(0, -1);
        }
        if(keystate[SDLK_DOWN]){
            world_map_gui.move_cursor(0, 1);
        }
    } else if(current_screen == GAME_SCREEN) {
        //THIS IS IMPORTANT, as it it turns out.
        if(game.is_initialized() && game.is_paused() == false) {
            if(keystate[SDLK_LEFT]){
                game.move_main_char(-1, 0);
            }
            if(keystate[SDLK_RIGHT]){
                game.move_main_char(1, 0);
            }
            if(keystate[SDLK_UP]){
                game.move_main_char(0, -1);
            }
            if(keystate[SDLK_DOWN]){
                game.move_main_char(0, 1);
            }
        }
    }
}
