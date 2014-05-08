#ifndef timer_h
#define timer_h


typedef struct time time;
struct time{
	float start_t, pause_t;
	short t_paused, t_started;
};

#endif