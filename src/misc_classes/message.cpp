/**
 *  MESSAGE.H
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

#include <message.h>

MessageBoard& MessageBoard::instance()
{
    static MessageBoard mb_instance; 
    return mb_instance;
}

MessageBoard::MessageBoard()
{
    max_messages = 20;
    message_index = 0;
    messages.resize(max_messages);
    messages[message_index] = "Testing.";
}

void MessageBoard::add_message(std::string message, int id)
{
    if(id == -1)
    {
        message_index += 1;
        if(message_index >= max_messages)
        {
            message_index = 0;
        }
        messages[message_index] = message;
    }
}

std::string MessageBoard::get_current_message()
{
    return messages[message_index];
}

std::string MessageBoard::get_message(int index)
{
    if(index >= 0 && index < max_messages)
    {
        return messages[index];
    }
}

