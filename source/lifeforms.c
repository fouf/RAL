#include "lifeforms.h"
#define Cell_create_pred(x, b) create_Cell(PREDATOR, x, b)
#define Cell_create_prey(x) create_Cell(PREY, x, 0)

Cell* create_Cell(Classification type, int breed, int starve)
{
	Cell* cell = malloc(sizeof(Cell));
	if(cell == NULL)
	{
		free(cell);
		cell = NULL;
		return NULL;
	}
	cell->Type = type;
	switch(type)
	{
		case PREDATOR:
		cell->Organism.predator.breed = breed;
		cell->Organism.predator.starve = starve;
		break;
		case PREY:
		cell->Organism.prey.breed = breed;
		break;
	}
	return cell;
}

void initialize_cells()
{
	cells = calloc(MAP_WIDTH * MAP_HEIGHT, sizeof(Cell *));
	if(cells == NULL)
	{
		setStatus("Cells failed to allocate memory.");
		return;
	}
	int i = 0, chance = 0;
	for(i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++)
	{
		chance = rand() % 4;
		if(chance > 0)
		{
			int class = rand() % 6;
			if(class > 0)
			{
				class = 1;
			}
			else
			{
				class = 0;
			}
			switch(class)
			{
				case PREDATOR:
					cells[i] = Cell_create_pred(simulation.g_pred_breed, simulation.g_pred_starve);
					simulation.total_pred++;
				break;
				case PREY:
					cells[i] = Cell_create_prey(simulation.g_prey_breed);
					simulation.total_prey++;
				break;
				default:
					cells[i] = NULL;
				break;
			}
		}
	}
}

void free_array(Cell **Array)
{
	int i = 0;
	if(Array != NULL)
	{
		for(i = 0; i < MAP_WIDTH*MAP_HEIGHT; i++)
		{
			if(Array[i] != NULL)
			{
				free(Array[i]);
			}
		}
	}
}

void cell_draw(int x, int y)
{
	SDL_Rect rect = {x*simulation.gwidth, y*simulation.gheight, simulation.gwidth, simulation.gheight};
	if(cells[y*MAP_WIDTH+x] != NULL)
	{
		SDL_FillRect(Application->Window, &rect, c_border);
		rect.x++;
		rect.y++;
		rect.w -= 2;
		rect.h -= 2;
		SDL_FillRect(Application->Window, &rect, (cells[(y*MAP_WIDTH) + x]->Type) == PREDATOR ? c_pred : c_prey);
	}
	else
	{
		SDL_FillRect(Application->Window, &rect, c_border);
		rect.x++;
		rect.y++;
		rect.w -= 2;
		rect.h -= 2;
		SDL_FillRect(Application->Window, &rect, c_space);
	}
}

void cleanup_cells()
{
	free_array(cells);
	free(cells);
	cells = NULL;
	initialize_cells();
}

