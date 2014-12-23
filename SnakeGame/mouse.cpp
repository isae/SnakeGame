#include "mouse.h"

mouse::mouse()
{
	
	//srand (time(NULL));
	mmouse = load_image("imgs/mouse.bmp");
	x = rand()%FIELD_WIDTH;
	y = rand()%FIELD_HEIGHT;
	
}

// get the position of the mouse
std::pair<int,int> mouse::get_pos(){
	return std::pair<int,int>(x,y);
}

void mouse::respawn(){
	x = rand()%FIELD_WIDTH;
	y = rand()%FIELD_HEIGHT;
	if((*field)[x][y].state!=0){
		respawn();}
}

void mouse::draw_image(){
	(*field)[x][y].surface=mmouse;
	(*field)[x][y].isChanged=true;
}
