#include "timer.h"
void time_default(time* timer)
{
	timer->t_paused = 0; timer->t_started = 0;
	timer->pause_t = 0; timer->start_t = 0;
}
void time_start(time* timer)
{
	if(!timer->t_paused)
	{
		timer->t_paused = 0; timer->t_started = 1;
		timer->start_t = SDL_GetTicks();
	}
}
void time_stop(time* timer)
{
	timer->t_paused = 0;
	timer->t_paused = 0;
}

void time_pause(time* timer)
{
	if((!timer->t_paused) && (timer->t_started))
	{
		timer->t_paused = 1;
		timer->pause_t = SDL_GetTicks() - timer->start_t;
	}
}

void time_unpause(time* timer)
{
	if((timer->t_paused))
	{
		timer->t_paused = 0;
		timer->start_t = SDL_GetTicks() - timer->pause_t;
	}
}

unsigned int time_ticks(time* timer)
{
	if(timer->t_started)
	{
		
		if(timer->t_paused)
			return timer->pause_t;
		
		return SDL_GetTicks() - timer->start_t;
	}
	return 0;
}