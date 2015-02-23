/**
 *  MESSAGE_CONTROLLER.CPP
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

#include <game.h>

void Game::add_message(std::string message)
{
    message_index += 1;
    if(message_index >= max_messages)
    {
        message_index = 0;
    }
    messages[message_index] = message;
}

std::string Game::get_current_message()
{
    return messages[message_index];
}

std::string Game::get_message(int index)
{
    if(index >= 0 && index < max_messages)
    {
        return messages[index];
    }
}
