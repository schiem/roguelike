#include "GUI.h"

void GUI::OnRender()
{
    canvas.main_char.update_dungeon(canvas.get_dungeon());
    canvas.main_char.perform_action_cont();
    canvas.refresh(); 
    TileMatrix tm = canvas.get_matrix();

    for(int i = 0; i < tm.size(); i++) {
        for(int j = 0; j < tm[i].size(); j++) {
            drawChr(j, i, tm[i][j].char_count, ascii, screen, white);
        }
    }

    SDL_Flip(screen);
    SDL_Delay(50);
}
