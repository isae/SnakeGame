#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sdl_functions.h"

class welcomeScreen
{
public:
    welcomeScreen(SDL_Surface *screen);
    void draw_screen(SDL_Surface* surface, SDL_Event event);
    void init_screen(SDL_Surface* surface);
    void delete_surfaces();
	int choice;

private:
    SDL_Surface* welcome_screen;
    SDL_Surface* welcome_screen_single;
    SDL_Surface* welcome_screen_pvp;
    SDL_Surface* welcome_screen_pvc;
    SDL_Surface* welcome_screen_pvcc;
    SDL_Surface* welcome_screen_pvccc;
    int mx, my;

};


#endif // WELCOMESCREEN_H
