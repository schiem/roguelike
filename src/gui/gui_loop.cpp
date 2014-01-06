/**
 *  GUI_LOOP.CPP
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


void GUI::OnLoop() {
    pt::ptime clock2;

    perform_action_cont();
    if(game.is_initialized()) {
        clock2 = pt::microsec_clock::local_time();
        game.act((clock2 - game_clock).total_milliseconds());
        game.refresh();
        game.run_spawners();
        //game.run_enemies();
    }

    game_clock = clock2;
}
