#include "gui.h"

bool GUI::OnInit()
{
    //initialize stuff
    SDL_Init(SDL_INIT_EVERYTHING);

    //Set up screen
    screen = SDL_SetVideoMode( 800, 600, 32, SDL_SWSURFACE );
	
	//Load ascii characters
    asciiBase = SDL_LoadBMP( "resources/ascii.bmp" );
    
	//Create an optimized image
    ascii = SDL_DisplayFormat( asciiBase );

    //Free the old image
    SDL_FreeSurface( asciiBase );
    
	//Map the color key
    Uint32 colorkey = SDL_MapRGB( ascii->format, 0xFF, 0, 0xFF );

    //Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );
    SDL_SetColorKey( ascii, SDL_SRCCOLORKEY, colorkey );

    return true;
}
