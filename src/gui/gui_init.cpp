/**
 *  GUI_INIT.CPP
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

namespace pt = boost::posix_time;
namespace gregorian = boost::gregorian;

bool GUI::OnInit() {

    //initialize stuff
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO );

    //Set up screen
    screen = SDL_SetVideoMode( 800, 600, 32, SDL_SWSURFACE );

    //Load ascii characters
    asciiBase = SDL_LoadBMP( DATADIR "/ascii.bmp" );
    
    if(asciiBase == 0)
    {
        cout<<"No font file found.  Please place ascii.bmp in the data directory."<<endl;
        return false;
    }
    //Create an optimized image
    ascii = SDL_DisplayFormat( asciiBase );

    //Free the old image
    SDL_FreeSurface( asciiBase );

    //Map the color key
    Uint32 colorkey = SDL_MapRGB( ascii->format, 0xFF, 0, 0xFF );

    //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );

    game_clock = pt::microsec_clock::local_time();

    return true;
}
