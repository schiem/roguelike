#include "gui.h"

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
void GUI::perform_action_press(SDLKey key) {
    //This is a pointer to a const value. The pointer can be modified, but the
    //value at the pointer cannot be modified from this name. Future Seth:
    //remember that 'const' is left-binding.
    Chunk * current_chunk = canvas.get_chunk();
    Main_Character* main_char = &canvas.main_char;
    Tile* current_tile = current_chunk->get_tile(main_char->get_depth(),
            main_char->get_y(), main_char->get_x()); 
    int main_char_depth = main_char->get_depth();

    switch (key) {
        case SDLK_u:
            if (main_char_depth-1 >= -1) {
                if(*current_tile == UP_STAIR) {
                    main_char->set_depth(main_char_depth - 1);
                    main_char->set_x(current_chunk->get_up_stair(main_char_depth)[0]);
                    main_char->set_y(current_chunk->get_up_stair(main_char_depth)[1]);
                }
            }
            break;
        case SDLK_d:
            if (main_char_depth+1 < current_chunk->get_depth()) {
                if(*current_tile == DOWN_STAIR) {
                    main_char->set_depth(main_char_depth + 1);
                    main_char->set_x(current_chunk->get_down_stair(main_char_depth)[0]);
                    main_char->set_y(current_chunk->get_down_stair(main_char_depth)[1]);
                }
            }
        default:
            break;
    }
}


void GUI::perform_action_cont() {
    SDL_PumpEvents();
    Uint8* keystate = SDL_GetKeyState(NULL);

    if(keystate[SDLK_LEFT]){
        char_move(-1, 0);
    }
    if(keystate[SDLK_RIGHT]){
        char_move(1, 0);
    }
    if(keystate[SDLK_UP]){
        char_move(0, -1);
    }
    if(keystate[SDLK_DOWN]){
        char_move(0, 1);
    }
}

/**
 * PRE: Will be passed the desired change in x and y coordinates
 * POST: Will change the character's coordinates to match this
 *
 * TODO Maybe defer this to canvas or something else.
 */
void GUI::char_move(int col_change, int row_change) {
    int row = canvas.main_char.get_y();
    int col = canvas.main_char.get_x();
    /**
    cout<<"ROW: "<<row<<" COL: "<<col<<endl;
    cout<<"WIDTH: "<<canvas.get_chunk()->width<<" HEIGHT: "<<canvas.get_chunk()->height;
    */
    int next_col = col + col_change;
    int next_row = row + row_change;
    if((next_col < 0) ||  (next_col >= canvas.get_chunk()->width) || 
            (next_row < 0) ||  (next_row >= canvas.get_chunk()->height)) {
        col += col_change;
        row += row_change;
    } else if (canvas.get_chunk()->get_tile(canvas.main_char.get_depth(),
                next_row, next_col)->can_be_moved_through) {
        col += col_change;
        row += row_change;
    }
    canvas.main_char.set_x(col);
    canvas.main_char.set_y(row);
}
