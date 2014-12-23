#include "welcomescreen.h"

welcomeScreen::welcomeScreen(SDL_Surface* screen)
{
    welcome_screen_single = NULL;
    welcome_screen_pvp = NULL;
    welcome_screen_pvc = NULL;
    welcome_screen_pvcc = NULL;
    welcome_screen_pvccc = NULL;

    welcome_screen_single = load_image("imgs/welcome_screen/welcome_screen_single.bmp");
    welcome_screen_pvp = load_image("imgs/welcome_screen/welcome_screen_pvp.bmp");
    welcome_screen_pvc = load_image("imgs/welcome_screen/welcome_screen_pvc.bmp");
    welcome_screen_pvcc = load_image("imgs/welcome_screen/welcome_screen_pvcc.bmp");
    welcome_screen_pvccc = load_image("imgs/welcome_screen/welcome_screen_pvccc.bmp");

    // Position in midst of screen
    mx = screen->w/2 - welcome_screen_single->w/2;
    my = screen->h/2 - welcome_screen_single->h/2;
	choice=0;

}


void welcomeScreen::init_screen(SDL_Surface* surface){
	apply_surface(mx, my, welcome_screen_single, surface, NULL);
	choice=0;
}

// Draws the menu message on the screen, and depending on mouse position
// displays one of the possible screens
void welcomeScreen::draw_screen(SDL_Surface* surface, SDL_Event event)
{
    //std::cout<<"here";
    // Handle the look of the dead screen
	// apply_surface(mx, my, welcome_screen_single, surface, NULL);
    if(event.type == SDL_KEYDOWN){
		switch(event.key.keysym.sym){
		case(SDLK_RIGHT):{
			switch(choice){
			case 0:{apply_surface(mx, my, welcome_screen_pvp, surface, NULL);choice=1;break;}
			case 1:{apply_surface(mx, my, welcome_screen_pvc, surface, NULL);choice=2;break;}
			case 2:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}
			case 3:{apply_surface(mx, my, welcome_screen_pvccc, surface, NULL);choice=4;break;}
			case 4:{apply_surface(mx, my, welcome_screen_single, surface, NULL);choice=0;break;}}
			break;}
		case(SDLK_UP):{
			switch(choice){
			case 0:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}
			case 1:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}
			case 2:{apply_surface(mx, my, welcome_screen_pvccc, surface, NULL);choice=4;break;}
			case 3:{apply_surface(mx, my, welcome_screen_single, surface, NULL);choice=0;break;}
			case 4:{apply_surface(mx, my, welcome_screen_pvc, surface, NULL);choice=2;break;}}
			break;}
		case(SDLK_DOWN):{
			switch(choice){
			case 0:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}
			case 1:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}
			case 2:{apply_surface(mx, my, welcome_screen_pvccc, surface, NULL);choice=4;break;}
			case 3:{apply_surface(mx, my, welcome_screen_single, surface, NULL);choice=0;break;}
			case 4:{apply_surface(mx, my, welcome_screen_pvc, surface, NULL);choice=2;break;}}
			break;}
		case(SDLK_LEFT):{
			switch(choice){
			case 0:{apply_surface(mx, my, welcome_screen_pvccc, surface, NULL);choice=4;break;}
			case 1:{apply_surface(mx, my, welcome_screen_single, surface, NULL);choice=0;break;}
			case 2:{apply_surface(mx, my, welcome_screen_pvp, surface, NULL);choice=1;break;}
			case 3:{apply_surface(mx, my, welcome_screen_pvc, surface, NULL);choice=2;break;}
			case 4:{apply_surface(mx, my, welcome_screen_pvcc, surface, NULL);choice=3;break;}}
			break;}
		}
        

    // Refresh the screen!
    SDL_Flip(surface);
    }
}

// Delete all assigned values of the object
void welcomeScreen::delete_surfaces(){
    SDL_FreeSurface(welcome_screen_single);
    SDL_FreeSurface(welcome_screen_pvp);
    SDL_FreeSurface(welcome_screen_pvc);
    SDL_FreeSurface(welcome_screen_single);
    SDL_FreeSurface(welcome_screen_pvcc);
    SDL_FreeSurface(welcome_screen_pvccc);
}
