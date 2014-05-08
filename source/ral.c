#include "sdl.c" // SDL file - holds various functions for setting up SDL
#include "timer.c" // Timer file - holds various functions for calibrating the timer
#include "sim.c" // Simulation file - holds various functions for the simulation

app *Application;
time *Gametimer;
SDL_Surface *Logo;
SDL_Surface *Icon;
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application = malloc(sizeof(app)); // Struct holding SDL basics..
	Gametimer = malloc(sizeof(time)); // Struct holding timer information..
	Logo = SDL_LoadBMP("logo.bmp");
	Icon = SDL_LoadBMP("icon.bmp");
	// error handling
	float delta_time = 0.02f;
	if(Application == NULL || Gametimer == NULL)
	{
		free(Application); free(Gametimer);
		return EXIT_FAILURE;
	}
	Application->hInst = hInstance;
	initialize_sdl(Application, 800, 600, SDL_DOUBLEBUF | SDL_HWSURFACE);
	time_default(Gametimer);
	time_start(Gametimer);
	//

		simulation_setup(Application, Gametimer);
		
	//
	Application->clear = Application->running = 1;
	if(Icon)
	{
		SDL_WM_SetIcon(Icon, NULL);
	}
	while(Application->running)
	{
		srand(SDL_GetTicks());
		process_events(Application);
		if(Application->active)
		{
			if(Application->clear == 1)
			{
				if(SDL_FillRect(Application->Window, NULL, c_menu_bg) == -1)
				{
					fprintf(stderr, "SDL_FillRect failed.");
				}
				Application->clear = 0;
			}
			//
				simulation_tick(delta_time);
			//
			
			
			if(SDL_Flip(Application->Window) == -1)
			{
				fprintf(stderr, "SDL_Flip failed.\n");
				Application->running = 0;
			}
		}
	}
	
	clean_up(Application);
	free(Gametimer);
	if(Logo != NULL)
	{
		SDL_FreeSurface(Logo);
	}
	if(Icon != NULL)
	{
		SDL_FreeSurface(Icon);
	}
	return EXIT_SUCCESS;
}
