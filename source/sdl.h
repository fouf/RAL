#ifndef sdl_h
#define sdl_h
#include "resource.h"
#include "util.h" // Utility file - holds misc. functions

typedef struct app app;
struct app{
	SDL_Surface* Window; SDL_Event events;
	short running, active, focus, clear;
	int Flags, r_w, r_h;
	HWND hWnd;
	HINSTANCE hInst;
};
Uint32 colour_black;
Uint32 colour_white;
#endif

