#include "config.c"
#include "lifeforms.c"
#include "sim.h"
app *Application;
time *Gametimer;
extern SDL_Surface* Logo;
void simulation_newgeneration();

int amount;
int simulation_init()
{
	simulation_cleanup();
	initialize_cells();
	simulation.update = simulation.generations = 1;
	simulation.g_pred_breed = 15;
	simulation.g_pred_starve = 5;
	simulation.g_prey_breed = 5;
	if(Application->r_w % MAP_WIDTH != 0 || Application->r_h % MAP_HEIGHT != 0)
	{
		MessageBox(Application->hWnd, "Width or height do not divide evenly into resolution.\n", "Attention", MB_ICONEXCLAMATION);
	}
	simulation.gwidth = floor(Application->r_w / MAP_WIDTH);
	simulation.gheight = floor(Application->r_h / MAP_HEIGHT);

	init_dialog();
	if(!simulation.Config)
	{
		fprintf(stderr, "Configuration dialog failed to load. GetLastError: %s \n", (char*)GetLastError());
		return 0;
	}
	setStatus("Varibles set up successfully.");
	time_start(Gametimer);
	time_pause(Gametimer);
	simulation.generation = simulation.tickdelay = simulation.total_prey = simulation.total_pred = amount = 0;
	cleanup_cells();
	return 1;
}

void simulation_setup(app *App, time *timer) // Args to edit simulation
{
	Application = App;
	Gametimer = timer;
}
void update_graph()
{

	int x = simulation.generation - amount;
	if(x > Application->Window->w)
	{
		amount += Application->Window->w;
	}
	float ratio = GRAPH_HEIGHT / (MAP_WIDTH * MAP_HEIGHT);
	int y_prey = Application->r_h + GRAPH_HEIGHT - (simulation.total_prey * ratio);
	int y_pred = Application->r_h + GRAPH_HEIGHT - (simulation.total_pred * ratio);
	SDL_Rect rect0 = {x, Application->r_h, 2, GRAPH_HEIGHT};
	SDL_FillRect(Application->Window, &rect0, c_menu_bg);
	SDL_Rect rect = {x, y_pred, 3, 3};
	SDL_FillRect(Application->Window, &rect, c_pred);
        SDL_Rect rect2 = {x, y_prey, 3, 3};
	SDL_FillRect(Application->Window, &rect2, c_prey);
	
	SDL_Rect rect3 = {x + 2, Application->r_h, 1, GRAPH_HEIGHT};
	SDL_FillRect(Application->Window, &rect3, RGB(0,0,0));
	
}
void handle_wm(UINT Msg)
{
	switch(Msg)
	{
		case WM_COMMAND:
		switch(LOWORD(Application->events.syswm.msg->wParam))
		{
			case IDM_EXIT1:
				Application->running = 0;
			break;					
			case IDM_START1:
				if(simulation.state == 0)
				{
					if(Gametimer->t_paused)
					{
						time_unpause(Gametimer);							
					}
					if(simulation_init())
					{
						setStatus("PvP initialized.");
						Application->clear = 1;
						simulation.state = 1;
					}
				}
			break;
		case IDM_ABOUT1:
			MessageBox(Application->hWnd, "Created by Fahid Jarmash\nHorton High School 2010-2011", "About", MB_OK | MB_ICONINFORMATION);
		break;
		case IDM_HELP1:
			// HELP
		break;
		}
	}
}


void simulation_tick(float delta_time) // Simulation loop
{
	SDL_Rect r_dest;
	switch(simulation.state)
	{
		case 0:
			// Configuration
			r_dest.x = (Application->Window->w / 2) - Logo->w / 2;
			r_dest.y = (Application->Window->h / 2) - Logo->h / 2;
			SDL_BlitSurface(Logo, NULL, Application->Window, &r_dest);	                      // Menu
			simulation.generation = simulation.total_prey = simulation.total_pred = amount = 0;
		break;
		case 1:
		if(time_ticks(Gametimer) > simulation.tickdelay)
		{
			if(!Gametimer->t_paused)
			{
				int i = 0;
				for(i = 0; i < simulation.generations; i++)
				{
					simulation_process(delta_time);
				}
				simulation.update = 1;
			}
			time_start(Gametimer);
		}
		if(simulation.update)
		{
			simulation_render(delta_time);
			simulation.update = 0;
		}
		
		// Update Variables
		char buff[512];
		int pbreed, pstarve, prbreed;
		pbreed = pstarve = prbreed = 0;
		pbreed = SendDlgItemMessage(simulation.Config, IDC_PRED_BREED, TBM_GETPOS, 0,0);
		pstarve = SendDlgItemMessage(simulation.Config, IDC_PRED_STARVE, TBM_GETPOS, 0,0);
		prbreed = SendDlgItemMessage(simulation.Config, IDC_PREY_BREED, TBM_GETPOS, 0,0);
		if(pbreed != simulation.g_pred_breed)
		{
			simulation.g_pred_breed = pbreed;
			snprintf(buff, 512, "%d", simulation.g_pred_breed);
			SetDlgItemText(simulation.Config, IDC_PRED_BREED_V, buff);
		}
		if(pstarve != simulation.g_pred_starve)
		{
			simulation.g_pred_starve = pstarve;
			snprintf(buff, 512, "%d", simulation.g_pred_starve);
			SetDlgItemText(simulation.Config, IDC_PRED_STARVE_V, buff);	
		}
		if(prbreed != simulation.g_prey_breed)
		{
			
			simulation.g_prey_breed = prbreed;		
			snprintf(buff, 512, "%d", simulation.g_prey_breed);
			SetDlgItemText(simulation.Config, IDC_PREY_BREED_V, buff);		
		}

		//
		break;
	}
}

void simulation_newgeneration()
{
	int* neighbours;
	int preys[8], predators[8], empty[8], rand_array[MAP_WIDTH*MAP_HEIGHT], prey_count, pred_count, empty_count, random, i, p;
	prey_count = pred_count = empty_count = random = i = p = 0;
	// array of indicies !!!!!!!!
	for(i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++)
	{
		rand_array[i] = i;
	}
	for(i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++)
	{
		int randz = rand() % (MAP_WIDTH * MAP_HEIGHT);
		rand_array[i] = rand_array[randz]; rand_array[randz] = i;
	}
	for(i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		if(cells[rand_array[i]] != NULL)
		{
			neighbours = simulation_check(rand_array[i]);
			for(p = 0; p < 7; p++)
			{
				if(cells[neighbours[p]] != NULL)
				{
					if(cells[neighbours[p]]->Type == PREY)
					{
						preys[prey_count] = neighbours[p];
						prey_count++;
					}
					else if(cells[neighbours[p]]->Type == PREDATOR)
					{
						predators[pred_count] = neighbours[p];
						pred_count++;
					}
				}
				else if(cells[neighbours[p]] == NULL)
				{
					empty[empty_count] = neighbours[p];
					empty_count++;
				}
			}
			switch(cells[rand_array[i]]->Type)
			{
				case PREDATOR:
					if(cells[rand_array[i]]->Organism.predator.starve < 0)
					{
						free(cells[rand_array[i]]);
						cells[rand_array[i]] = NULL;
						simulation.total_pred--;
					}
					else
					{						
						if(prey_count > 0)
						{
							random = rand() % (prey_count);
							free(cells[preys[random]]);
							cells[preys[random]] = NULL;
							cells[preys[random]] = Cell_create_pred((cells[rand_array[i]]->Organism.predator.breed - 1), simulation.g_pred_starve);
							// That cell becomes this predator Starve reset to 0, if multiple of breed, leave offspring
							simulation.total_prey--;
							if(cells[rand_array[i]]->Organism.predator.breed >= 0)
							{
								free(cells[rand_array[i]]); // Free / Nullify old cel
								cells[rand_array[i]] = NULL;
							}
							else
							{
								cells[rand_array[i]]->Organism.predator.breed = simulation.g_pred_breed;
								cells[rand_array[i]]->Organism.predator.starve = simulation.g_pred_starve;
								simulation.total_pred++;
							}
						}
						else if(empty_count > 0) // No prey nearby
						{
							random = rand() % (empty_count);
							cells[empty[random]] = Cell_create_pred((cells[rand_array[i]]->Organism.predator.breed - 1), (cells[rand_array[i]]->Organism.predator.starve - 1)); // Predator moves to empty cell
							if(cells[rand_array[i]]->Organism.predator.breed >= 0)
							{
								free(cells[rand_array[i]]);
								cells[rand_array[i]] = NULL;
							}
							else
							{
								cells[rand_array[i]]->Organism.predator.breed = simulation.g_pred_breed;
								simulation.total_pred++;
							}
						}
						else if(empty_count == 0) // Surrounded by predators
						{
							// can't move
							cells[rand_array[i]]->Organism.predator.starve -= 1;
						}
					}
					break;
				
				case PREY:
					if(empty_count > 0) // empty area near by!
					{
						random = rand() % (empty_count);					
						cells[empty[random]] = Cell_create_prey((cells[rand_array[i]]->Organism.prey.breed - 1)); // Prey moves to empty cell
						if(cells[rand_array[i]]->Organism.prey.breed >= 0)
						{
							free(cells[rand_array[i]]);
							cells[rand_array[i]] = NULL;						
						}
						else
						{
							cells[rand_array[i]]->Organism.prey.breed = simulation.g_prey_breed;
							simulation.total_prey++;
						}
					}
				break;
			}
			free(neighbours); prey_count = pred_count = empty_count = random = 0;
			int k = 0;
			for(k = 0; k < 7; k++)
			{
				preys[k] = 0;
				predators[k] = 0;
				empty[k] = 0;
			}
		}
	}
	update_graph();
}
void simulation_process(float delta_time)
{
	simulation_newgeneration();	
	simulation.generation++;
	char buff[512]; snprintf(buff, 512, "Generation: %d", (int)simulation.generation);
	SetDlgItemText(simulation.Config, IDC_GENERATION, buff);
	snprintf(buff, 512, "Predators: %d", (int)simulation.total_pred);
	SetDlgItemText(simulation.Config, IDC_PREDATORS, buff);
	snprintf(buff, 512, "Preys: %d", (int)simulation.total_prey);
	SetDlgItemText(simulation.Config, IDC_PREYS, buff);
}
void simulation_render(float delta_time)
{
	if(cells != NULL)
	{
		int i = 0, j = 0;
		for(i = 0; i < MAP_HEIGHT; i++)
		{
			for(j = 0; j < MAP_WIDTH; j++)
			{
				cell_draw(j, i);
			}
		}
	}
	update_graph();
}

void simulation_cleanup()
{
	if(cells != NULL || cells != NULL)
	{
		simulation.generation = simulation.total_prey = simulation.total_pred = amount = 0;
		setStatus("Cells reset.");
		free_array(cells);
		free(cells);
		cells = NULL;
	}
}
