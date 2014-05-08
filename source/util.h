#ifndef util_h
#define util_h

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <windows.h>
#include <math.h>
COLORREF c_pred = RGB(32, 32, 255);
COLORREF c_prey = RGB(0, 255, 0);
COLORREF c_space = RGB(25, 25, 25);
COLORREF c_border = RGB(25, 25, 25);
COLORREF c_menu_bg = RGB(200, 200, 200);
const float GRAPH_HEIGHT = 75;
SDL_Surface *load_bmp(char *filename)
{
	SDL_Surface *loaded_img = SDL_LoadBMP(filename);
	if(loaded_img == NULL)
	{
		fprintf(stderr, "Could not load file: %s\n", filename);
		return NULL;
	}
	SDL_Surface *img = SDL_DisplayFormat(loaded_img);
	SDL_FreeSurface(loaded_img);
	if(img == NULL)
	{
		fprintf(stderr, "SDL_DisplayFormat failed on: %s\n", filename);
		return NULL;
	}
	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 0xFF, 0x00, 0xFF));
	return img;
}
#endif
