#include "sdl.h"
#include "sim.h"
#include <commctrl.h>
void handle_wm(UINT Msg);
SDL_Surface* initialize_sdl(app* Application, int width, int height, int flags)
{
	InitCommonControls();
	Application->r_w = width;
	Application->r_h = height;
	SDL_Init(SDL_INIT_EVERYTHING);
	RECT rc;
	SIZE sizeWindow;
	rc.top = rc.left = 0;
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRect(&rc, WS_OVERLAPPED, 1);
	sizeWindow.cx = rc.right - rc.left;
	sizeWindow.cy = rc.bottom - rc.top;
	SDL_Surface *Window = SDL_SetVideoMode(sizeWindow.cx, sizeWindow.cy + GRAPH_HEIGHT, 24, flags);
	Application->Window = Window;
	Application->Flags = flags;
	if(Application->Window == NULL)
	{
		Application->running = 0;
		return NULL;
	}
	// Menu
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if(!SDL_GetWMInfo(&wmInfo))
	{
		fprintf(stderr, "Failed to get WM Info.\n");
		Application->running = 0;
		return Application->Window;
	}
	HWND hWnd = wmInfo.window;
	Application->hWnd = hWnd;
	HMENU hMenu = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(hWnd, hMenu);
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	//
	Application->running = Application->clear = 1;
	colour_black = SDL_MapRGB(Application->Window->format, 0x00, 0x00, 0x00);
	colour_white = SDL_MapRGB(Application->Window->format, 0xFF, 0xFF, 0xFF);
	
	SDL_FillRect(Application->Window, NULL, SDL_MapRGB(Application->Window->format, 0x10, 0x10, 0x10));
	SDL_WM_SetCaption("PvP", NULL);
	return Application->Window;
}

int process_events(app* Application)
{
	int height, width;
	while(SDL_PollEvent(&Application->events))
	{
		switch(Application->events.type)
		{
			case SDL_VIDEORESIZE:
				width = Application->events.resize.w;
				height = Application->events.resize.h;
				Application->Window = SDL_SetVideoMode(width, height, 24, Application->Flags);
				break;
			
			case SDL_ACTIVEEVENT:
				if(Application->events.active.state & SDL_APPINPUTFOCUS)	 //Input Focus
				{
					if(Application->events.active.gain)
					{
						Application->focus = 1;
					}
					else
					{
						Application->focus = 0;
					}
				}
				if(Application->events.active.state & SDL_APPACTIVE) // Iconified
				{
					if(Application->events.active.gain)
					{
						Application->active = 1;
					}
					else
					{
						Application->active = 0;
					}
				}
				break;
			case WM_SIZE:
				break;
				
			case SDL_QUIT:
				Application->running = 0;
				break;
			
			case SDL_SYSWMEVENT:
				handle_wm(Application->events.syswm.msg->msg);
			break;
		}
	}
	return 0;
}

void clean_up(app* Application)
{
	simulation_cleanup();
	SDL_FreeSurface(Application->Window);
	free(Application);
	Application = NULL;
}
