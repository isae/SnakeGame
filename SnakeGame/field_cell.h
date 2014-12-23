#ifndef FIELD_CELL
#define FIELD_CELL
#include "SDL.h"

struct field_cell{
	field_cell(SDL_Surface* surface, int state,bool isChanged);
	SDL_Surface* surface;
	bool isChanged;
	int state;


};

#endif