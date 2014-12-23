#ifndef SDL_FUNCTIONS_H
#define SDL_FUNCTIONS_H

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "field_cell.h"
#include <vector>
const int FRAMES_PER_SECOND = 10;
const int FIELD_WIDTH = 30;
const int FIELD_HEIGHT = 20;
const int MAX_MICES=50;
SDL_Surface* copySurface(SDL_Surface* src);
SDL_Surface *load_image( std::string filename );
void fill_background(SDL_Surface* the_background, SDL_Surface* the_screen, int offset_,int offset_y );
void fill_background(std::vector<std::vector<field_cell>>* field, SDL_Surface* screen);
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
void apply_surface(SDL_Surface* source, SDL_Surface* destination);
SDL_Surface *load_image( std::string filename );
bool detect_collision(std::pair<int,int> a, std::pair<int,int> b);
#endif // SDL_FUNCTIONS_H
