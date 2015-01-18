#ifndef ASCII_LIB_H
#define ASCII_LIB_H
#include <SDL/SDL.h>
#include <color_def.h>
#include <math.h>


// Print an ASCII character
void drawChr(int x, int y, int smiley, SDL_Surface* source, SDL_Surface* destination, Uint32 color);

// Print a string
void drawStr(int x, int y, const char stringy[], SDL_Surface* source, SDL_Surface* destination, Uint32 color);

// Print a number
void drawInt(int x, int y, int number, SDL_Surface* source, SDL_Surface* destination, Uint32 color, int size = 0);

void clearArea(int x, int y, int height, int width, SDL_Surface* destination, Uint32 color);

#endif
