#include "snake.h"
#include <fstream>
#include <cmath>
#include <queue>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


snake::snake(SDL_Surface *surface, string name)
{
	// Load initial images of playerSnake and sets position of head and tail
	mHead = load_image(string("imgs/").append(name).append("/head.bmp"));
	mBody = load_image(string("imgs/").append(name).append("/body.bmp"));
	mTail = load_image(string("imgs/").append(name).append("/tail.bmp"));
	mAngle = load_image(string("imgs/").append(name).append("/angle.bmp"));
	back =  load_image("imgs/background.bmp");

	SDL_Rect first;
	SDL_Rect second;
	SDL_Rect third;
	SDL_Rect fourth;
	SDL_Rect fifth;

	// all sides
	first.x = 0;
	first.y = 0;
	first.h = mHead->h/2;
	first.w = mHead->w/2;

	second.x = mHead->h/2;
	second.y = 0;
	second.h = mHead->h/2;
	second.w = mHead->w;

	third = second;
	third.x = mHead->h/2;
	third.y = mHead->w/2;
	third.h = mHead->h/2;
	third.w = mHead->w/2;


	fourth = second;
	fourth.x = 0;
	fourth.y = mHead->w/2;
	fourth.h = mHead->h/2;
	fourth.w = mHead->w/2;

	fifth = second;
	fifth.x = 0;
	fifth.y = 0;
	fifth.h = back->h;
	fifth.w = back->w;


	// Set the clips
	mclip_locations.insert(mclip_locations.begin()    , first);
	mclip_locations.insert(mclip_locations.begin() + 1, second);
	mclip_locations.insert(mclip_locations.begin() + 2, third);
	mclip_locations.insert(mclip_locations.begin() + 3, fourth);	
	mclip_locations.insert(mclip_locations.begin() + 4, fifth);

	for(int i=0; i<4;i++){
		SDL_Surface* surface = copySurface(back);
		apply_surface(0,0,mHead,surface,&mclip_locations.at(i));
		heads.insert(heads.end(),surface);
		surface = copySurface(back);
		apply_surface(0,0,mBody,surface,&mclip_locations.at(i));
		bodys.insert(bodys.end(),surface);
		surface = copySurface(back);
		apply_surface(0,0,mTail,surface,&mclip_locations.at(i));
		tails.insert(tails.end(),surface);
		surface = copySurface(back);
		apply_surface(0,0,mAngle,surface,&mclip_locations.at(i));
		angles.insert(angles.end(),surface);
	}
	live=0;
	mIsGrowing=false;
	dead=false;
	score=0;

	for(int i=0;i<FIELD_WIDTH;i++){
		vector<int> v;
		vector<bool> v1;
		vector<pair<int,int>> v2;
		dist.push_back(v);
		used.push_back(v1);
		p.push_back(v2);
		for(int j=0;j<FIELD_HEIGHT;j++){
			dist[i].push_back(100500);
			used[i].push_back(false);
			p[i].push_back(pair<int,int>(-1,-1));
		}}
}


void snake::make_move_arrows(SDL_Event event){
	pair<int,int> loc;
	
	
	//Set the proper consequence
	switch( event.key.keysym.sym )
	{
	case SDLK_UP:
		if(mlast_press != DOWN){
			loc=go_up();	
			mlast_press = UP;
			break;}

	case SDLK_DOWN:
		if(mlast_press != UP){
			loc=go_down();				
			mlast_press = DOWN;
			break;}

	case SDLK_LEFT:
		if(mlast_press != RIGHT){
			loc=go_left();	
			mlast_press = LEFT;
			break;}

	case SDLK_RIGHT:
		if(mlast_press != LEFT){
			loc=go_right();		
			mlast_press = RIGHT;
			break;}
	default:
		// Cases not button was pressed
		if(mlast_press == NONE)
		{
			//do nothing
		}
		else if(mlast_press == UP){
			go_up();	
		}
		else if(mlast_press == DOWN){
			go_down();	
		}
		else if(mlast_press == RIGHT){
			go_right();	
		}
		else if(mlast_press == LEFT){
			go_left();	
		}
		break;

	}
	

	if(mlast_press != NONE){
		remove_tail();
	}
}


void snake::make_move_wasd(SDL_Event event){
	std::pair<int,int> loc;

	//Set the proper consequence
	switch( event.key.keysym.sym )
	{
	case SDLK_w:
		if(mlast_press != DOWN){
			loc=go_up();	
			mlast_press = UP;
			break;}

	case SDLK_s:
		if(mlast_press != UP){
			loc=go_down();				
			mlast_press = DOWN;
			break;}

	case SDLK_a:
		if(mlast_press != RIGHT){
			loc=go_left();	
			mlast_press = LEFT;
			break;}

	case SDLK_d:
		if(mlast_press != LEFT){
			loc=go_right();		
			mlast_press = RIGHT;
			break;}
	default:
		// Cases not button was pressed
		if(mlast_press == NONE)
		{
			//do nothing
		}
		else if(mlast_press == UP){
			go_up();	
		}
		else if(mlast_press == DOWN){
			go_down();	
		}
		else if(mlast_press == RIGHT){
			go_right();	
		}
		else if(mlast_press == LEFT){
			go_left();	
		}
		break;

	}

	// Remove position of tail, but only if the snake moves
	if(mlast_press != NONE){
		remove_tail();
	}
}


bool snake::check_collision(mouse mouse){
	bool f=false;
	for(int i=0; i<parts.size();i++){
		std::pair<int,int> part = parts.at(i);
		if(detect_collision(mouse.get_pos(), part))
		f=true;
	}
	return f;
}

bool snake::touched_mouse(mouse myMouse){
	return detect_collision(myMouse.get_pos(), parts[0]);
}

bool snake::grow_snake(mouse myMouse){
	if(touched_mouse(myMouse)){
		mIsGrowing = true;
		return true;
	}
	else{
		return false;
	}

}


std::pair<int,int> loc;

std::pair<int,int> snake::go_up(){
	loc = parts.at(0);
	--loc.second;
	if(loc.second<0){dead=true;}
	else {
		if((*field)[loc.first][loc.second].state==2){dead=true;}
		parts.insert(parts.begin(), loc);
		(*field)[loc.first][loc.second].state=2;
		mdirections.insert(mdirections.begin(), UP);}
	return loc;
}

std::pair<int,int> snake::go_down(){
	loc = parts.at(0);
	++loc.second;
	if(loc.second>(FIELD_HEIGHT-1)){dead=true;}
	else {
		if((*field)[loc.first][loc.second].state==2){dead=true;}
		parts.insert(parts.begin(), loc);
		(*field)[loc.first][loc.second].state=2;
		mdirections.insert(mdirections.begin(), DOWN);}
	return loc;
}

std::pair<int,int> snake::go_left(){
	loc = parts.at(0);
	--loc.first;
	if(loc.first<0){dead=true;}
	else {
		if((*field)[loc.first][loc.second].state==2){dead=true;}
		parts.insert(parts.begin(), loc);
		(*field)[loc.first][loc.second].state=2;
		mdirections.insert(mdirections.begin(), LEFT);}
	return loc;
}

std::pair<int,int> snake::go_right(){
	loc = parts.at(0);
	++loc.first;
	if(loc.first>(FIELD_WIDTH-1)){dead=true;}
	else {
		if((*field)[loc.first][loc.second].state==2){dead=true;}
		parts.insert(parts.begin(), loc);
		(*field)[loc.first][loc.second].state=2;
		mdirections.insert(mdirections.begin(), RIGHT);}
	return loc;
}


// Drawing the snake on a surface
void snake::draw_snake(){
	int size = parts.size();

	std::pair<int,int> loc;
	int dir;
	int prev_dir;
	loc = parts.at(0);
	dir = mdirections.at(0);

	(*field)[loc.first][loc.second].surface=heads[dir];
	(*field)[loc.first][loc.second].isChanged=true;
	if(parts.size()>2){
		loc = parts.at(1);
		prev_dir = mdirections.at(0);
		dir = mdirections.at(1);
		(*field)[loc.first][loc.second].surface=bodys[dir];
		if(prev_dir!=dir){
			int angle;
			if( (prev_dir==0&&dir==1) || (prev_dir==3&&dir==2) ){angle=3;
			} else

			if((prev_dir==1&&dir==2) || (prev_dir==0&&dir==3) ){angle=0; 
			} else
			if((prev_dir==2&&dir==3) || (prev_dir==1&&dir==0)){angle=1; 				
			} else
			if((prev_dir==3&&dir==0) || (prev_dir==2&&dir==1) ){angle=2;
				
			} 
			(*field)[loc.first][loc.second].surface=angles[angle];
		} else {
			(*field)[loc.first][loc.second].surface=bodys[dir];
			
		} 
		(*field)[loc.first][loc.second].isChanged=true;
	}

	


	// Draw the tail
	loc = parts.at(parts.size()-1);
	dir = mdirections.at(mdirections.size()-2);
	(*field)[loc.first][loc.second].surface=back;
	(*field)[loc.first][loc.second].surface=tails[dir];
	(*field)[loc.first][loc.second].isChanged=true;
}

void snake::respawn(SDL_Surface* surface, int headX, int headY, int direction){
	parts.clear();
	mdirections.clear();
	// Give the begin location of snake
	parts.insert(parts.begin(), std::pair<int,int>(headX,headY));
	(*field)[headX][headY].isChanged=true;
	(*field)[headX][headY].state=2;
	switch(direction){
		case(0):{headX--; break;}
		case(1):{headY++; break;}
		case(2):{headX++; break;}
		case(3):{headY--; break;}
	}
	parts.insert(parts.begin()+1, std::pair<int,int>(headX,headY));
	(*field)[headX][headY].isChanged=true;
	(*field)[headX][headY].state=2;
	mdirections.insert(mdirections.begin(), direction);
	mdirections.insert(mdirections.begin(), direction);
	mlast_press = NONE;
}


int snake::get_distance(std::pair<int,int> source,std::pair<int,int> target){
	return (int)(sqrt(pow(((double)source.first-target.first),2)+pow(((double)source.second-target.second),2)));
}
int snake::get_distance(mouse myMouse){	
	return (int)(sqrt(pow(((double)parts[0].first-myMouse.x),2)+pow(((double)parts[0].second-myMouse.y),2)));
};


bool snake::can_go(std::pair<int,int> place){	
	if((place.first>=0&place.second>=0&place.first<FIELD_WIDTH&place.second<FIELD_HEIGHT)&&(*field)[place.first][place.second].state!=2)
	return true;
	else return false;
}

void snake::avoid_collision(){
	std::pair<int,int> head = parts.at(0);
	used_init();
	int d1 = count_freedom(head.first+1,head.second);
	used_init();
	int d2 = count_freedom(head.first-1,head.second);
	used_init();
	int d3 = count_freedom(head.first,head.second+1);
	used_init();
	int d4 = count_freedom(head.first,head.second-1);
	if(d1>=d2&d1>=d3&d1>=d4){
		go_right();
		return;
	}
	if(d2>=d1&d2>=d3&d2>=d4){
		go_left();
		return;
	}
	if(d3>=d1&d3>=d2&d3>=d4){
		go_down();
		return;
	}
	go_up();
	return;
}


void snake::dist_init(){
	for(int i=0;i<FIELD_WIDTH;i++){
		for(int j=0;j<FIELD_HEIGHT;j++){
			dist[i][j]=100500;
		}}
};
void snake::p_init(){
	for(int i=0;i<FIELD_WIDTH;i++){
		for(int j=0;j<FIELD_HEIGHT;j++){
			p[i][j] = pair<int,int>(-1,-1);
		}}
};
void snake::used_init(){
	for(int i=0;i<FIELD_WIDTH;i++){
		for(int j=0;j<FIELD_HEIGHT;j++){
			used[i][j]=false;
		}}
};

bool snake::in_danger(){
	int dir = mdirections[0];
	int s1 = parts[0].first;
	int s2 = parts[0].second;
	switch(dir){
	case UP:{s2--;
			break;}
	case DOWN:{s2++;
			break;}
	case LEFT:{s1--;
			break;}
	case RIGHT:{s1++;
			break;}
	}

	return (s1<0||s2<0||s1>(FIELD_WIDTH-1)||s2>(FIELD_WIDTH-1)||(*field)[s1][s2].state==2);

}

void snake::make_move_ai(vector<mouse> mices){
	live++;
	if(way.empty()|| live>6||(*field)[way[way.size()-1].first][way[way.size()-1].second].state!=1){
		live=0;
		find_way_to_mice(mices);
	}
	if(mices.size()!=0&!way.empty()/*&!in_danger()*/){
		pair<int,int> step = way[0];
		used_init();
		int freedom = count_freedom(step.first,step.second);
		if(can_go(step)&&freedom>=40){
			goTo(step);
			way.erase(way.begin());
		}else {avoid_collision();
			way.clear();
		}
	}else {avoid_collision();
		way.clear();
	}
	
	remove_tail();
}



void snake::goTo(std::pair<int,int> place){
	std::pair<int,int> loc = parts.at(0);
	parts.insert(parts.begin(), place);
	(*field)[place.first][place.second].state=2;
	if(loc.first-1==place.first && loc.second==place.second){			
		mdirections.insert(mdirections.begin(), LEFT);
	} else
	if(loc.first+1==place.first && loc.second==place.second){			
		mdirections.insert(mdirections.begin(), RIGHT);
	} else
	if(loc.first==place.first && loc.second-1==place.second){			
		mdirections.insert(mdirections.begin(), UP);
	} else
	if(loc.first==place.first && loc.second+1==place.second){			
		mdirections.insert(mdirections.begin(), DOWN);
	};
}

void snake::remove_tail(){
	if(mIsGrowing == false){
		loc = parts.at( parts.size()-1);
		(*field)[loc.first][loc.second].surface = back;
		(*field)[loc.first][loc.second].isChanged=true;
		(*field)[loc.first][loc.second].state=0;
		parts.erase(parts.begin()+ parts.size()-1);
		mdirections.erase(mdirections.begin()+ mdirections.size()-1);
	}
	mIsGrowing = false;
};

int snake::count_freedom(int x, int y){
	int result=0;
	if(x>(FIELD_WIDTH-1)||y>(FIELD_HEIGHT-1)||x<0||y<0||(*field)[x][y].state==2){
		return 0;
	}
	result=1;
	used[x][y]=true;
	if(x<(FIELD_WIDTH-1)&&!used[x+1][y])result+=count_freedom(x+1,y);
	if(x>0&&!used[x-1][y])result+=count_freedom(x-1,y);
	if(y<(FIELD_HEIGHT-1)&&!used[x][y+1])result+=count_freedom(x,y+1);
	if(y>0&&!used[x][y-1])result+=count_freedom(x,y-1);
	return result;
};

void snake::find_way_to_mice(std::vector<mouse> mices){
	way.clear();
	fstream fs;
	fs.open ("fields.txt", std::fstream::out);
	

	dist_init();
	used_init();
	p_init();	
	dist[parts[0].first][parts[0].second] = 0;


	pair<int,int> cur = parts[0];
	queue<pair<int,int>> q;
	q.push(cur);

	int x=cur.first;
	int y=cur.second;
	used[x][y]=true;
	int count =0;
	while(!q.empty()){
		cur = q.front();
		++count;
		x= cur.first;
		y = cur.second;		
		q.pop();
		
		if(x<(FIELD_WIDTH-1)){
			if(!used[x+1][y]){
				used[x+1][y]=true;
				dist[x+1][y]=dist[x][y]+1;
				p[x+1][y] = pair<int,int>(x,y);
				if((*field)[x+1][y].state!=2) q.push(pair<int,int>(x+1,y));
			}
		}
		if(x>0){
			if(!used[x-1][y]){
				used[x-1][y]=true;
				dist[x-1][y]=dist[x][y]+1;
				p[x-1][y] = pair<int,int>(x,y);
				if((*field)[x-1][y].state!=2) q.push(pair<int,int>(x-1,y));
			}
		}
		if(y<(FIELD_HEIGHT-1)){
			if(!used[x][y+1]){
				used[x][y+1]=true;
				dist[x][y+1]=dist[x][y]+1;
				p[x][y+1] = pair<int,int>(x,y);
				if((*field)[x][y+1].state!=2) q.push(pair<int,int>(x,y+1));
			}
		}
		if(y>0){
			if(!used[x][y-1]){
				used[x][y-1]=true;
				dist[x][y-1]=dist[x][y]+1;
				p[x][y-1] = pair<int,int>(x,y);
				if((*field)[x][y-1].state!=2) q.push(pair<int,int>(x,y-1));
			}
		}
		
		
	}


	int f=0;
	bool s = false;
	int dist = 100500;
	for(int i=0;i<mices.size();i++){
		pair<int,int> mice = make_pair(mices[i].x,mices[i].y);
		int p1=p[mice.first][mice.second].first;
		int p2=p[mice.first][mice.second].second;
		if(!(p1==-1&p2==-1)&&get_distance(mices[i])<dist){
			f=i;
			s=true;
			dist = get_distance(mices[i]);
			
		}
	}
	
	if(s&&mices.size()>0){
		x =(mices[f]).x;
		y =(mices[f]).y;
		pair<int,int> k;
		while(!(x==parts[0].first & y==parts[0].second)){
			way.insert(way.begin(), pair<int,int>(x,y));
			k = p[x][y];
			x = k.first;
			y = k.second;
		}}


}