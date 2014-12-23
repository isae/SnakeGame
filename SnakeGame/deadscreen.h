#ifndef DEADSCREEN_H
#define DEADSCREEN_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sdl_functions.h"


class deadScreen
{

public:
	deadScreen(SDL_Surface *screen);
	void draw_screen(SDL_Surface* surface, SDL_Event event);
	bool exit();


	// All three images for the deadScreen
	SDL_Surface* mdeadScreen;
	SDL_Surface* mdeadScreenNo;
	SDL_Surface* mdeadScreenYes;
	// the position of where to put the
	int mx;
	int my;
	int mw;
	int mh;
	bool mleft;

};

#endif // DEADSCREEN_H
