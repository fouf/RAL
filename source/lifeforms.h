#ifndef lifeforms_h
#define lifeforms_h
#include "util.h" // Utility file - holds misc. functions
typedef enum Classification Classification;
enum Classification
{
	PREDATOR,
	PREY
};
typedef struct
{
	Classification Type;
	union
	{
		struct
		{
			int breed;
		}prey;

		struct
		{
			int breed, starve;
		}predator;
	}Organism;
} Cell;

void cleanup_cells();
#endif
