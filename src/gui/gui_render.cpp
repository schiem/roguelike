#include "gui.h"

void GUI::OnRender()
{
    if(current_screen == MAP_SCREEN) {
        std::vector<std::vector<MapTile> > map = world_map.get_map();
        for(size_t i = 0; i < map.size(); i++) {
            for(size_t j = 0; j < map[i].size(); j++) {
                drawChr(j, i, map[i][j].char_count, ascii, screen, map[i][j].color);
            }
        }
        drawStr(0, 0, "Press ENTER to continue.", ascii, screen, WHITE);
    } else {
        perform_action_cont();
        canvas.refresh();
        TilePointerMatrix tm = canvas.get_matrix();
        TileMatrix tl = canvas.get_top_layer();
        for(size_t i = 0; i < tm.size(); i++) {
            for(size_t j = 0; j < tm[i].size(); j++) {
                if(tm[i][j]->visible) {
                    drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);
                    if(tl[i][j].visible){
                        drawChr(j, i, tl[i][j].char_count, ascii, screen, tl[i][j].color);
                    }
                } else {
                    //Draw an empty tile
                    drawChr(j, i, 0, ascii, screen, 0);
                }
            }
        }
    }

    SDL_Flip(screen);
    SDL_Delay(50);
}
