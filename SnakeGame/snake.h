#ifndef SNAKE_H
#define SNAKE_H
#include <iostream>
#include <vector>
#include "sdl_functions.h"
#include "mouse.h"
using namespace std;
struct snake
{
public:

	
	snake(SDL_Surface *surface, string folder);
	void make_move_arrows(SDL_Event event);
	void make_move_wasd(SDL_Event event);

	vector <int> mdirections;	
	bool check_collision(mouse m);	
	// virtual void make_move(SDL_Event event, SDL_Surface *surface)=0;
	//bool wall_collision;	
	void redraw_all(SDL_Surface* screen);
	bool grow_snake(mouse m);
	int live;
	//bool touched_self();
	//vector <SDL_Rect> return_partLocation();
	unsigned int score;
	bool dead;//=(
	
	pair<int,int> go_up();
	pair<int,int> go_down();
	pair<int,int> go_left();
	pair<int,int> go_right();	
	vector<pair<int,int>> parts;
	void respawn(SDL_Surface* surface, int headX, int headY, int direction);
	vector<vector<field_cell>>* field;


	

	int current_direction;
	void draw_snake();
	vector<SDL_Surface*> heads;
	vector<SDL_Surface*> bodys;
	vector<SDL_Surface*> tails;
	vector<SDL_Surface*> angles;

	int count_freedom(int x, int y);

	//some ai things	
	void remove_tail();
	vector<pair<int,int>> way;

	bool  in_danger();
	
	vector<vector<int>> dist;
	void dist_init();
	vector<vector<pair<int,int>>> p;	
	void p_init();
	vector<vector<bool>> used;
	void used_init();

	int get_distance(pair<int,int> source,pair<int,int> target);
	int get_distance(mouse m);
	bool can_go(pair<int,int> place);
	void find_way_to_mice(vector<mouse> mices);
	void goTo(pair<int,int> place);
	void snake::make_move_ai(vector<mouse> mices);
	void avoid_collision();

	// the image holders
	SDL_Surface*  mHead;
	SDL_Surface*  mTail;
	SDL_Surface*  mBody;
	SDL_Surface*  mAngle;	
	SDL_Surface*  back;

	// An enumeration containing the possible directions of the snake
	enum direction
	{
		RIGHT,
		DOWN,
		LEFT,
		UP,
		NONE
	};

	int mlast_press;

	// Tracks if the snake is supposed to grow
	bool mIsGrowing;


	// Stores the clip locations
	vector <SDL_Rect> mclip_locations;

	// A vector that keeps track of the direction of all elements of the snake


	bool touched_mouse(mouse myMouse);
};

#endif 
