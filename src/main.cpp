#include <DungeonBuilder.h>
#include <terrain_defs.h>
#include <Character.h>
#include <SDL/SDL.h>
#include <def.h>
#include <ASCII_Lib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

SDL_Event event;
SDL_Surface* screen = NULL;
SDL_Surface* asciiBase = NULL;
SDL_Surface* ascii = NULL;
DungeonBuilder dungeon(40, 40);
Main_Character main_char(100, 1, 1, 3); 


void initialize(){

	//Start SDL
    SDL_Init( SDL_INIT_EVERYTHING );

    //Set up screen
    screen = SDL_SetVideoMode( 800, 480, 32, SDL_SWSURFACE );
	
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

	
}
bool get_input(){

	while (SDL_PollEvent( &event ) )
		{
			switch (event.type)
			{
				case SDL_QUIT:
					return false;
					break;
				case SDL_KEYDOWN:
					main_char.perform_action(event.key.keysym.sym);
					break;
				default:
					break;
			}
		}
	return true;
}

void act(){
	if (!dungeon.is_initialized()){
	dungeon.build_pblind_dungeon(4,1,1);
    dungeon.initialize();
	}

}

void display(){
	if (dungeon.is_initialized()){
        //what the dick is that number?
		dungeon.print(ascii, screen, 16777215);
	}
	main_char.display_character(main_char.get_char(), ascii, screen, 16777215);
	
	SDL_Flip (screen);
	SDL_Delay(50);
}

void cleanup(){
	SDL_FreeSurface (ascii);
	SDL_Quit();

}


int main(int argc, char* args[] )
{
	initialize();
	bool running = true;
    /* Loop until an SDL_QUIT event is found */
	while( running )
    {
		running = get_input();
		act();
		display();
	}
	cleanup();
	return 0;	
}

