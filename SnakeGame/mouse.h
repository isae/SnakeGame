#ifndef MOUSE_H
#define MOUSE_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "sdl_functions.h"
#include "field_cell.h"

using namespace std;
struct mouse
{
public:
	mouse();
	pair<int,int> get_pos();
	void respawn();	
	void draw_image();	
	vector<vector<field_cell>>* field;
	SDL_Surface* mmouse;

	int x;
	int y;

};



#endif // MOUSE_H
