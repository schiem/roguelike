/**
 *  @file AUDIO_MENU.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
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

#include "menu.h"

AudioMenu::AudioMenu(int padding, Tile _border, Game* _game) : Menu(padding, _border)
{
    id = 1;
    game = _game;
    next_screen = GAME_SCREEN;
    title = "Silly, there's no audio yet.  Maybe Seth can compose something later...";
    options.push_back("Back");
}

Menu* AudioMenu::make_selection()
{

    return new EscapeMenu(1, BLOCK_WALL, game);
}
