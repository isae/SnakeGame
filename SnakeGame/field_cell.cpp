#include "field_cell.h"

field_cell::field_cell(SDL_Surface* surface,  int state,bool isChanged=true){
	this->surface=surface;
	this->state=state;
	this->isChanged=isChanged;
}