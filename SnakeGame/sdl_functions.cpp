#include "sdl_functions.h"
//#include "field_cell.h"
//#include <vector>

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	// Holds offsets
	SDL_Rect offset;

	// Get offsets
	offset.x = x;
	offset.y = y;

	// Blit
	SDL_BlitSurface( source, clip, destination, &offset );
}

void apply_surface(SDL_Surface* source, SDL_Surface* destination)
{

	// Blit
	SDL_BlitSurface( source, NULL, destination, NULL);
}

// Fill the background using input image
void fill_background(SDL_Surface* the_background, SDL_Surface* the_screen,int offset_x = 0,int offset_y = 0)
{
	for (int i = offset_x; i < the_screen->w - offset_x-1; i += the_background->w)
	{
		for (int j = offset_y; j < the_screen->h - offset_y-1; j += the_background->h)
		{
			apply_surface(i,j,the_background, the_screen);
		}
	}
}
void fill_background(std::vector<std::vector<field_cell>>* field,SDL_Surface* screen)
{
	SDL_Rect rect;
	for(int i=0;i<FIELD_WIDTH;i++)
	for(int j=0;j<FIELD_HEIGHT;j++){
		if((*field)[i][j].isChanged){
			rect.x=10+FIELD_WIDTH*i;
			rect.y=10+FIELD_WIDTH*j;
			rect.w=FIELD_WIDTH;
			rect.h=FIELD_WIDTH;
			apply_surface(rect.x,rect.y,(*field)[i][j].surface,screen,NULL);}
		(*field)[i][j].isChanged=false;
	}
}

SDL_Surface* copySurface(SDL_Surface* src)
{
	return SDL_ConvertSurface(src, src->format, SDL_SWSURFACE);
}


SDL_Surface *load_image( std::string filename )
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	if( optimizedImage != NULL){
		//Map the color key
		Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF );
		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );


		//Return the optimized image
		return optimizedImage;
	}

	return loadedImage;

}

bool detect_collision(std::pair<int,int> a,
std::pair<int,int> b)
{
	return a.first==b.first && a.second==b.second;
}









