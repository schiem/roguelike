#include "Display.h"

using namespace std;
typedef std::vector<std::vector<Tile> > TileMatrix;

Display::Display()
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

    //canvas.refresh();
}

bool Display::get_input()
{
	while (SDL_PollEvent( &event ) )
		{
			switch (event.type)
			{
				case SDL_QUIT:
					return false;
					break;
				case SDL_KEYDOWN:
                    //catch_regen_dungeon(event.key.keysym.sym);
					canvas.main_char.perform_action_press(event.key.keysym.sym);
					break;
				default:
					break;
			}
		}
	return true;
}

void Display::refresh()
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

void Display::cleanup()
{
	SDL_FreeSurface (ascii);
	SDL_Quit();
}
