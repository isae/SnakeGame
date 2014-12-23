#ifndef PROGRAMLOOP_H
#define PROGRAMLOOP_H
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sdl_functions.h"
#include "timer.h"
#include "deadscreen.h"
#include "welcomescreen.h"
#include "field_cell.h"
#include "snake.h"
#include "mouse.h"

class programLoop
{
public:
	programLoop();
	void run();
	int prev_choice;
	std::vector<std::vector<field_cell>> field;
	std::vector<mouse> mices;
	void mouse_respawn(mouse *m, vector<snake>* snakes);
	void draw_score(snake *snake, int x, int y, string name, SDL_Surface* screen);
	SDL_Surface* cell;
	int counter;
	bool time_to_stop(vector<snake> snakes);
private:
	SDL_Surface* screen;
	// Stores the event data
	SDL_Event event;
	void refresh_all();
	void play(vector<snake> snakes);
	void single_play();
	void pvp_play();
	void pvc_play();
	void pvcc_play();
	void pvccc_play();
	void dead_screen(int game);
	void clean_up();


	Timer fps;
};

#endif // PROGRAMLOOP_H
