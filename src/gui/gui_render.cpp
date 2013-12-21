#include "gui.h"

void GUI::OnRender()
{
    canvas.main_char.perform_action_cont();
    canvas.refresh(); 
    TilePointerMatrix tm = canvas.get_matrix();

    for(size_t i = 0; i < tm.size(); i++) {
        for(size_t j = 0; j < tm[i].size(); j++) {
            if(tm[i][j]->visible) {
                drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);
            } else {
                //Draw an empty tile
                //TODO THIS IS STUPID DESIGN. Tile logic shouldn't be up this
                //high.
                drawChr(j, i, 0, ascii, screen, 0);
            }
        }
    }

    SDL_Flip(screen);
    SDL_Delay(50);
}
