#ifndef sim_h
#define sim_h
#include "util.h" // Utility file - holds misc. functions
#include "lifeforms.h"

int MAP_WIDTH = 200, MAP_HEIGHT = 200;
Cell **cells = {0};
typedef struct simulate simulate;
struct simulate
{
	int state; // State 0 = Configuration, State 1 = Simulating
	int update, gheight, gwidth, tickdelay, generations, g_pred_breed, g_prey_breed, g_pred_starve, total_prey, total_pred;
       	unsigned long long int generation, predators, prey;
	// 0 = Do not update, 1 = Update
	HWND Config, Graph;
};

simulate simulation;
int simulation_events();
void simulation_cleanup();
void simulation_render(float delta_time);
void simulation_process(float delta_time);
int simulation_init();
void simulation_populate();
int* simulation_check(int index);

int* simulation_check(int index)
{
	// RIGHT //
	int current_level = (MAP_WIDTH) * ceil((double)(index+1) / MAP_WIDTH);
	int *nearby = calloc(8, sizeof(int));
	
	if(index + 1 > current_level - 1)
	{
		nearby[0] = index - (MAP_WIDTH-1);
	}
	else
	{
		nearby[0] = index+1;
	}

	// LEFT //

	if(index - 1 < current_level - MAP_WIDTH)
	{
		nearby[1] = index + (MAP_WIDTH-1);
	}
	else
	{
		nearby[1] = index-1;
	}
	// ABOVE //
	
	if(index - MAP_WIDTH >= 0)
	{
		nearby[2] = index - MAP_WIDTH;
	}
	else
	{
		nearby[2] = index + (MAP_WIDTH * (MAP_HEIGHT-1));      	
	}
	
	// BELOW //
	
	if((index + MAP_WIDTH) <= MAP_HEIGHT * MAP_WIDTH-1)
	{
		nearby[3] = index+MAP_HEIGHT;
	}
	else
	{
		nearby[3] = index - (MAP_WIDTH * (MAP_HEIGHT-1));
	}

	/// UPPER LEFT //
	
	if(index == 0)	// Upper left out of bounds.
	{
		nearby[4] = MAP_WIDTH*MAP_HEIGHT-1;
	}
	else if(index - MAP_WIDTH - 1 < current_level - (2*MAP_WIDTH))       // Upper left out of bounds to the left Correct.
	{
		nearby[4] = index - 1;
	}
	else if(index - MAP_WIDTH - 1 < 0)        // Upper left out of bounds above
	{
		nearby[4] = index + (MAP_WIDTH * (MAP_HEIGHT-1)) - 1;
	}
	else    // Not out of bounds
	{
		nearby[4] = index - MAP_WIDTH - 1;
	}

	/// UPPER RIGHT ///
	
	if(index == MAP_WIDTH-1)	// Upper right out of bounds.
	{
		nearby[5] = MAP_WIDTH*MAP_HEIGHT-MAP_WIDTH;
	}
	else if(index - MAP_WIDTH + 1 > current_level - MAP_WIDTH - 1)         // Upper right out of bounds to the right 
	{
		nearby[5] = index - MAP_WIDTH - (MAP_WIDTH - 1);
	}
	else if(index - MAP_WIDTH + 1 < 0)        // Upper right out of bounds above
	{
		nearby[5] = index + (MAP_WIDTH * (MAP_HEIGHT-1)) + 1;
	}
	else    // Not out of bounds
	{
		nearby[5] = index - MAP_WIDTH + 1;
	}

	// BOTTOM RIGHT //
	
	if(index == MAP_WIDTH*MAP_HEIGHT-1)	// Bottom right out of bounds.
	{
		nearby[6] = 0;
	}
	else if(index + MAP_WIDTH + 1 > current_level + MAP_WIDTH - 1)        // Bottom right out of bounds to the right 
	{
		nearby[6] = index + 1;
	}
	else if(index + MAP_WIDTH + 1 > MAP_WIDTH*MAP_HEIGHT - 1)        // Bottom right out of bounds below
	{
		nearby[6] = (int)(index - ((MAP_WIDTH) * ceil((double)(index+1) / MAP_WIDTH-1)) + 1);
	}
	else    // Not out of bounds
	{
		nearby[6] = index + MAP_WIDTH + 1;
	}

	// BOTTOM LEFT //
	
	if(index == MAP_WIDTH*MAP_HEIGHT-MAP_WIDTH)	// Bottom left out of bounds.
	{
		nearby[7] = MAP_WIDTH-1;
	}
	else if(index + MAP_WIDTH - 1 < current_level)       // Bottom left out of bounds to the left 
	{
		nearby[7] = index + 2*MAP_WIDTH - 1;
	}
	else if(index + MAP_WIDTH - 1 > MAP_WIDTH*MAP_HEIGHT-1)        // Bottom left out of bounds below
	{
		nearby[7] = index - (MAP_WIDTH * (MAP_HEIGHT-1)) - 1;
	}
	else    // Not out of bounds
	{
		nearby[7] = index + MAP_WIDTH - 1;
	}
	return nearby;
}
#endif
