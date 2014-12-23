#include "programloop.h"
#include <string>
#include <sstream>

#include <fstream>
programLoop::programLoop()

{

	//The background images
	screen = NULL;

	

	// Start SDL
	SDL_Init( SDL_INIT_EVERYTHING);
	_putenv("SDL_VIDEO_CENTERED=center");

	TTF_Init();

	// Set up the screen
	screen = SDL_SetVideoMode((FIELD_WIDTH*30+20), (FIELD_HEIGHT*30+20), 32,
	SDL_SWSURFACE);
	SDL_Surface* fence = load_image("imgs/fence.bmp");
	apply_surface(0,0,fence,screen,NULL);

	// Set the window caption
	SDL_WM_SetCaption("2Player Snake", NULL);
	prev_choice=0;
	cell = load_image("imgs/background.bmp");
	SDL_Surface* surface; 
	//todo поправить картинку, снизу на один пиксель меньше
	surface = load_image("imgs/background.bmp");
	for(int i=0;i<FIELD_WIDTH;i++){
		std::vector<field_cell> vector ;
		field.push_back(vector);
		for(int j=0;j<FIELD_HEIGHT;j++){			
			field[i].push_back(field_cell(surface,0,true));
		}}
	fill_background(&field,screen);
	counter=0;
}

void programLoop::mouse_respawn(mouse *m, vector<snake>* snakes){
	bool f= true;
	m->respawn();
	for(int i=0;i<(*snakes).size();i++){
		if(snakes->at(i).check_collision(*m)){
			f=false;
			break;}
	}
	if(!f){
		mouse_respawn(m, snakes);
	}
}
// Runs the program starting with the welcome prompt
void programLoop::run(){
	if(prev_choice==0){
		welcomeScreen myWelcomeScreen(screen);
		myWelcomeScreen.init_screen(screen);

		// Refresh the screen
		SDL_Flip(screen);

		bool quit = false;
		bool play;
		

		
		
		
		
		// Start running with the welcome prompt
		while(quit == false)
		{

			while(SDL_PollEvent(&event))
			{

				myWelcomeScreen.draw_screen(screen, event);
				

				if(event.key.keysym.sym  == SDLK_RETURN)
				{
					play = true;
					quit = true;
					break;
				}
				else if(event.type == SDL_QUIT)
				{
					clean_up();
					exit(0);
				}
			}

		}

		if(play == true){
			switch(myWelcomeScreen.choice){
			case 0:{single_play();break;}
			case 1:{pvp_play();break;}
			case 2:{pvc_play();break;}
			case 3:{pvcc_play();break;}
			case 4:{pvccc_play();break;}
			}
		}
		else
		{
			// Program terminates
		}
	} else 
	switch(prev_choice){
	case 1:{single_play();break;}
	case 2:{pvp_play();break;}
	case 3:{pvc_play();break;}
	case 4:{pvcc_play();break;}
	case 5:{pvccc_play();break;}
	}
}

void programLoop::refresh_all(){
	mices.clear();
	for(int i=0;i<FIELD_WIDTH;i++)
	for(int j=0;j<FIELD_HEIGHT;j++){
		field[i][j].surface=cell;
		field[i][j].isChanged=true;
		field[i][j].state=0;
	}
}

void programLoop::single_play()
{
	prev_choice=1;
	refresh_all();
	fill_background(&field,screen);
	SDL_Flip(screen);
	fstream fs;
	fs.open ("debug.txt", std::fstream::out);

	counter=0;
	snake mySnake(screen,"playersnake");
	mySnake.field= &field;
	mySnake.respawn(screen,1,0,0);
	bool quit = false;
	while(!mySnake.dead && quit == false){
		for(int i=0;i<mices.size();i++){
			mices[i].draw_image();
		}
		mySnake.draw_snake();	
		fps.start();		
		fill_background(&field,screen);
		SDL_Flip(screen);
		
		
		for(int i=0;i<mices.size();i++){
			mouse *cur = &mices[i];
			if(mySnake.grow_snake(*cur)){			
				mySnake.score+=  fps.get_ticks();
				fs<<endl;
				field[cur->x][cur->y].state= 0;
				mices.erase(mices.begin()+i);
				fs<<endl;

			}
		}

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT)
			{
				clean_up();
				exit(0);
			}

		}
		
		counter++;
		//If we want to cap the frame rate
		if( ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
		{
			//Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
		}


		mySnake.make_move_arrows(event);
		if(counter%25==0&& mices.size()<MAX_MICES){
			mouse newbie;
			newbie.field=&field;
			newbie.respawn();
			(field)[newbie.x][newbie.y].state=1;
			mices.push_back(newbie);
		}
	}

	deadScreen myDead(screen);

	apply_surface(myDead.mx, myDead.my, myDead.mdeadScreenYes, screen, NULL);
	//myDead.draw_screen(screen, event);	
	draw_score(&mySnake,150,150,"Your",screen);
	SDL_Flip(screen);
	quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);	

			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				fs.close();
				exit(0);
			}
		}
	}

}

void programLoop::pvp_play()
{
	prev_choice=2;
	refresh_all();
	fill_background(&field,screen);	 	
	SDL_Flip(screen);
	counter=0;
	snake mySnake(screen, "playersnake");
	snake mySnake2(screen,"playersnake2");
	mySnake.field= &field;
	mySnake.respawn(screen,1,0,0);
	mySnake2.field= &field;
	mySnake2.respawn(screen, 28,(FIELD_HEIGHT-1),2);

	bool quit = false;
	while(!mySnake2.dead&&!mySnake.dead&& quit == false){		
		fps.start();
		for(int i=0;i<mices.size();i++){
			mices[i].draw_image();
		}
		mySnake.draw_snake();
		mySnake2.draw_snake();
		fill_background(&field,screen);
		SDL_Flip(screen);
		
		
		for(int i=0;i<mices.size();i++){
			mouse *cur = &mices[i];
			if(mySnake.grow_snake(*cur)){			
				mySnake.score+=  fps.get_ticks();
				field[cur->x][cur->y].state= 0;
				mices.erase(mices.begin()+i);

			} else if(mySnake2.grow_snake(*cur)){			
				mySnake2.score+=  fps.get_ticks();
				field[cur->x][cur->y].state= 0;
				mices.erase(mices.begin()+i);

			}
		}

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT)
			{
				clean_up();
				exit(0);
			}

		}
		
		counter++;
		//If we want to cap the frame rate
		if( ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
		{
			//Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
		}
		mySnake.make_move_arrows(event);
		mySnake2.make_move_wasd(event);
		if(counter%15==0&& mices.size()<MAX_MICES){
			mouse newbie;
			newbie.field=&field;
			newbie.respawn();
			(field)[newbie.x][newbie.y].state=1;
			mices.push_back(newbie);
		}

	}
	
	
	
	
	deadScreen myDead(screen);
	apply_surface(myDead.mx, myDead.my, myDead.mdeadScreenYes, screen, NULL);
	
	draw_score(&mySnake,150,150,"Blue",screen);
	draw_score(&mySnake2,150,200,"Green",screen);
	SDL_Flip(screen);
	quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);	

			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				exit(0);
			}
		}
	}
	


}

void programLoop::pvc_play()
{
	prev_choice=3;
	refresh_all();
	fill_background(&field,screen);	 	
	SDL_Flip(screen);
	counter=0;
	snake mySnake(screen, "playersnake");
	snake compSnake(screen,"playersnake2");
	mySnake.field= &field;
	mySnake.respawn(screen,1,0,0);
	compSnake.field= &field;
	compSnake.respawn(screen, 28,(FIELD_HEIGHT-1),2);

	bool quit = false;
	while(!compSnake.dead&&!mySnake.dead&& quit == false){		
		fps.start();
		for(int i=0;i<mices.size();i++){
			mices[i].draw_image();
		}
		mySnake.draw_snake();
		compSnake.draw_snake();
		fill_background(&field,screen);
		SDL_Flip(screen);
		
		
		for(int i=0;i<mices.size();i++){
			mouse *cur = &mices[i];
			if(mySnake.grow_snake(*cur)){			
				mySnake.score+=  fps.get_ticks();
				mices.erase(mices.begin()+i);

			} else if(compSnake.grow_snake(*cur)){			
				compSnake.score+=  fps.get_ticks();
				mices.erase(mices.begin()+i);

			}
		}

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT)
			{
				clean_up();
				exit(0);
			}

		}
		
		counter++;
		//If we want to cap the frame rate
		if( ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
		{
			//Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
		}
		mySnake.make_move_arrows(event);
		compSnake.make_move_ai(mices);
		if(counter%15==0&& mices.size()<MAX_MICES){
			mouse newbie;
			newbie.field=&field;
			newbie.respawn();
			(field)[newbie.x][newbie.y].state=1;
			mices.push_back(newbie);
		}
	}

	
	deadScreen myDead(screen);
	apply_surface(myDead.mx, myDead.my, myDead.mdeadScreenYes, screen, NULL);	
	draw_score(&mySnake,150,150,"Blue",screen);
	draw_score(&compSnake,150,200,"Green",screen);
	SDL_Flip(screen);
	quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);	

			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				exit(0);
			}
		}
	}


	

}

bool programLoop::time_to_stop(vector<snake> snakes){
	for(unsigned int i=0;i<snakes.size();i++){
		if(snakes[i].dead) return  true;
	}
	return false;
}

void programLoop::pvcc_play()
{
	prev_choice=4;
	vector<snake> snakes;
	refresh_all();
	fill_background(&field,screen);	 	
	SDL_Flip(screen);
	counter=0;
	snake mySnake(screen, "playersnake");
	snake compSnake(screen,"playersnake2");
	snake compSnake2(screen,"aisnake1");
	snakes.push_back(mySnake);
	snakes.push_back(compSnake);
	snakes.push_back(compSnake2);
	for(int i=0;i<snakes.size();i++){
		snakes[i].field = &field;
	}
	snakes[0].respawn(screen,1,0,0);
	snakes[1].respawn(screen, 28,(FIELD_HEIGHT-1),2);
	snakes[2].respawn(screen, 1,(FIELD_HEIGHT-1),0);


	bool quit = false;
	while(!time_to_stop(snakes)&& quit == false){		
		fps.start();
		for(int i=0;i<mices.size();i++){
			mices[i].draw_image();
		}
		for(int i=0;i<snakes.size();i++){
			snakes[i].draw_snake();
		}
		fill_background(&field,screen);
		SDL_Flip(screen);
		
		
		for(int i=0;i<mices.size();i++){
			mouse *cur = &mices[i];
			for(int j=0;j<snakes.size();j++){
				if(snakes[j].grow_snake(*cur)){
					snakes[j].score+=  fps.get_ticks();
					mices.erase(mices.begin()+i);
				}
			}
		}

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT)
			{
				clean_up();
				exit(0);
			}

		}
		
		counter++;
		//If we want to cap the frame rate
		if( ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
		{
			//Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks());
		}
		snakes[0].make_move_arrows(event);
		snakes[1].make_move_ai(mices);
		snakes[2].make_move_ai(mices);
		if(counter%7==0&& mices.size()<MAX_MICES){
			mouse newbie;
			newbie.field=&field;
			newbie.respawn();
			(field)[newbie.x][newbie.y].state=1;
			mices.push_back(newbie);
		}
	}

	
	deadScreen myDead(screen);
	apply_surface(myDead.mx, myDead.my, myDead.mdeadScreenYes, screen, NULL);;
	draw_score(&snakes[0],150,150,"Green",screen);
	draw_score(&snakes[1],150,200,"Blue",screen);;
	draw_score(&snakes[2],150,250,"Yellow",screen);

	SDL_Flip(screen);
	quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);	

			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				exit(0);
			}
		}
	}


	
}

void programLoop::draw_score(snake* snake, int x, int y, string name, SDL_Surface* screen){	
	std::stringstream ss;
	ss<<name<< " snake's score: "<<(*snake).score;
	string t= ss.str();
	deadScreen myDead(screen);
	SDL_Surface *text;
	TTF_Font *font;
	font = TTF_OpenFont("FreeSans.ttf", 28);
	SDL_Color text_color = {255, 255, 255};
	text = TTF_RenderText_Solid(font,
	t.c_str(), text_color);     
	apply_surface(x, y, text, screen, NULL);
}
void programLoop::pvccc_play()
{
	prev_choice=5;
	vector<snake> snakes;
	refresh_all();
	fill_background(&field,screen);	 	
	SDL_Flip(screen);
	counter=0;
	snake mySnake(screen, "playersnake");
	snake compSnake(screen,"playersnake2");
	snake compSnake2(screen,"aisnake1");
	snake compSnake3(screen,"aisnake2");
	snakes.push_back(mySnake);
	snakes.push_back(compSnake);
	snakes.push_back(compSnake2);
	snakes.push_back(compSnake3);
	for(int i=0;i<snakes.size();i++){
		snakes[i].field = &field;
	}
	snakes[0].respawn(screen,1,0,0);
	snakes[1].respawn(screen, 28,(FIELD_HEIGHT-1),2);
	snakes[2].respawn(screen, 1,(FIELD_HEIGHT-1),0);
	snakes[3].respawn(screen, 28,0,2);


	bool quit = false;
	while(!time_to_stop(snakes)&& quit == false){		
		fps.start();
		for(int i=0;i<mices.size();i++){
			mices[i].draw_image();
		}
		for(int i=0;i<snakes.size();i++){
			snakes[i].draw_snake();
		}
		fill_background(&field,screen);
		SDL_Flip(screen);
		
		
		for(int i=0;i<mices.size();i++){
			mouse *cur = &mices[i];
			for(int j=0;j<snakes.size();j++){
				if(snakes[j].grow_snake(*cur)){
					snakes[j].score+=  fps.get_ticks();
					mices.erase(mices.begin()+i);
				}
			}
		}

		while(SDL_PollEvent(&event)){


			if(event.type == SDL_QUIT)
			{
				clean_up();
				exit(0);
			}

		}
		
		counter++;
		//If we want to cap the frame rate
		if( ( fps.get_ticks() < 1000 / FRAMES_PER_SECOND ) )
		{
			//Sleep the remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) -fps.get_ticks()-10);
		}
		snakes[0].make_move_arrows(event);
		snakes[1].make_move_ai(mices);
		snakes[2].make_move_ai(mices);
		snakes[3].make_move_ai(mices);
		if(counter%3==0&& mices.size()<MAX_MICES){
			mouse newbie;
			newbie.field=&field;
			newbie.respawn();
			(field)[newbie.x][newbie.y].state=1;
			mices.push_back(newbie);
		}

	}

	
	deadScreen myDead(screen);
	apply_surface(myDead.mx, myDead.my, myDead.mdeadScreenYes, screen, NULL);;
	draw_score(&snakes[0],150,150,"Green",screen);
	draw_score(&snakes[1],150,200,"Blue",screen);;
	draw_score(&snakes[2],150,250,"Yellow",screen);
	draw_score(&snakes[3],150,300,"Black",screen);

	SDL_Flip(screen);
	quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);	

			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				exit(0);
			}
		}
	}


}

void programLoop::dead_screen(int game){

	deadScreen myDead(screen);
	myDead.draw_screen(screen, event);
	bool quit = false;
	while(quit == false)
	{
		while(SDL_PollEvent(&event)){
			myDead.draw_screen(screen, event);
			
			if(event.key.keysym.sym == SDLK_RETURN){
				if(myDead.exit())
				{
					clean_up();
					exit(0);
				}
				else{
					SDL_Delay(200);
					SDL_PollEvent(&event);
					run();

				}


				break;
			}
			if(event.type == SDL_QUIT )
			{
				clean_up();
				// Quit the loop
				exit(0);
			}
		}
	}
}

void programLoop::clean_up(){

	SDL_FreeSurface(screen);
	SDL_Quit();

}