/**
 *  GUI_RENDER.CPP
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

void GUI::OnRender()
{
    if(current_screen == MAP_SCREEN) {
        std::vector<std::vector<MapTile> > map_canvas = world_map_gui.get_canvas();
        for(size_t i = 0; i < map_canvas.size(); i++) {
            for(size_t j = 0; j < map_canvas[i].size(); j++) {
                drawChr(j, i, map_canvas[i][j].char_count, ascii, screen, map_canvas[i][j].color);
            }
        }
        drawStr(0, 48, "Use the arrow keys to move the cursor.", ascii, screen, WHITE);
        drawStr(0, 49, "Press ENTER to spawn on the selected map tile.", ascii, screen, WHITE);
    } else {
        if(!game.is_initialized()) {
            game.init(world_map_gui.get_map(), world_map_gui.get_selected_chunk());
        }
        TilePointerMatrix tm = game.get_canvas();
        std::vector<TilePoint> tl = game.get_top_layer();
        for(size_t i = 0; i < tm.size(); i++) {
            for(size_t j = 0; j < tm[i].size(); j++) {
                //If the tile is visible, render it fully.
                if(tm[i][j]->visible) {
                    drawChr(j, i, tm[i][j]->char_count, ascii, screen, tm[i][j]->color);
                //If the tile is not visible, but has been seen, render it in
                //grey.
                } else if(tm[i][j]->seen) {
                    drawChr(j, i, tm[i][j]->char_count, ascii, screen, VERY_DARK_GRAY);
                    //We probably shouldn't draw the enemy layer on non-visible
                    //tiles.
                } else {
                    //Draw an empty tile
                    drawChr(j, i, 0, ascii, screen, 0);
                }
            }
        }

        //Draw enemies, etc.
        Tile current_tile;
        IntPoint current_point;
        for(size_t i = 0; i < tl.size(); i++) {
            current_tile = tl[i].tile;
            current_point = tl[i].loc;
            if(tm[current_point.row][current_point.col]->visible) {
                drawChr(current_point.col, current_point.row,
                        current_tile.char_count, ascii, screen, current_tile.color);
            }
        }
    }

    SDL_Flip(screen);
    SDL_Delay(50);
}
