#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "LTexture.h" 
#include "LTimer.h"
#include "LScore.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture* chooseTexture[2] = {0};
LTexture* bgTexture = NULL;
LTexture* roleTexture = NULL;
LTexture* lifeheart = NULL;
LTexture* cloudTexture[2] = {0}; 
LTexture* scoreTexture = NULL;
LTexture* gameoverTexture = NULL;
TTF_Font *gFont = NULL;
LTimer timer;
LScore score;

SDL_Rect gChooseClip;
SDL_Rect gRoleClips[ 3 ][ 4 ];
SDL_Rect gBackgroundClip;
SDL_Rect heartClips;
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 560;
const SDL_Color textColor = { 0, 0, 0 };
const int TH = 106;
const int TW = 80;
int bg_num;
int role_num = 0;

bool init()
{
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		if( TTF_Init() == -1 )
		{
			printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
			success = false;
		}
		//Create window
		gWindow = SDL_CreateWindow( "Jumping Eevee", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
			for(int i = 0; i<2; i++)
			{
				chooseTexture[i] = new Background;
				if( chooseTexture[i] == NULL )
				{
					printf( "Choose background could not be created! SDL Error: %s\n", SDL_GetError() );
					success = false;
				}	
			}
		
			bgTexture = new Background;
			if( bgTexture == NULL )
			{
				printf( "Background could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			gameoverTexture = new Background;
			if( gameoverTexture == NULL)
			{
				printf( "Gameover background could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			roleTexture = new Roles;
			if( roleTexture == NULL )
			{
				printf( "Role could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			scoreTexture = new Text;
			if( scoreTexture == NULL )
			{
				printf( "Score text could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			
			for(int i=0; i<2; i++)
			{
				cloudTexture[i] = new decorations(i);
				if( cloudTexture[i] == NULL )
				{
					printf( "cloudTexture[ %d ] could not be created! SDL Error: %s\n", i, SDL_GetError() );
					success = false;
				}
			}
		}
		
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;
	gFont = TTF_OpenFont( "font/lazy.ttf", 28 );
	if(gFont == NULL)
	{
		printf( "Failed to load font!\n" );
		success = false;
	}
	string name = "img/choose_";
	for(int i=0; i<2; i++)
	{
		string namei = name;
		namei += to_string(i);
		namei += ".png";
		if( !chooseTexture[i]->load( gRenderer, namei ) )
		{
			printf( "Failed to load choose backgrounds!\n" );
			success = false;
		}
	}
	gChooseClip.x = 0;
	gChooseClip.y = 0;
	gChooseClip.w = 230;
	gChooseClip.h = 230;
	if( !bgTexture->load( gRenderer, "img/background.png" ) )
	{
		printf( "Failed to load backgrounds!\n" );
		success = false;
	}
	else
	{
		gBackgroundClip.x = 0;
		gBackgroundClip.y = 0;
		gBackgroundClip.w = SCREEN_WIDTH;
		gBackgroundClip.h = SCREEN_HEIGHT;
	}
	if( !gameoverTexture->load( gRenderer, "img/Game_over.png" ))
	{
		printf( "Failed to load GO backgrounds!\n" );
		success = false;
	}
	if( !roleTexture->load( gRenderer, "img/roles.png" ) ){
		printf( "Failed to load roles!\n" );
		success = false;
	}
	else
	{
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<4; j++)
			{
				gRoleClips[i][j].w = EBUSIZE;
				gRoleClips[i][j].h = EBUSIZE;
				gRoleClips[i][j].x = i*EBUSIZE;
				gRoleClips[i][j].y = j*EBUSIZE;
			}
		}	
	}
	
	for(int i=0; i<2; i++)
	{
		if(!cloudTexture[i]->load( gRenderer, "img/cloud.png" ) ){
			printf( "Failed to load things!\n" );
			success = false;
		}
	}
	
	if( !dynamic_cast<Text*>(scoreTexture)->loadFromRenderedText( gRenderer, "Score: 0", textColor , gFont) )
	{
		
		printf( "Unable to render score texture!\n\n" );
		success = false;
	}

	return success;
}

void close()
{	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool checkCollision( SDL_Rect thing, SDL_Rect role )
{
    if( (thing.y + thing.h) <= role.y || thing.y >= (role.y + role.h) || (thing.x + thing.w) <= role.x || thing.x >= (role.x + role.w))
		return false;
    return true;
}



int colresult(LTexture* thing, LTexture* role)
{
    if(checkCollision( dynamic_cast<Things*>(thing)->get_mCollider(),  dynamic_cast<Roles*>(role)->get_mCollider()))
    {
    	if(thing->get_type() == 2)
    	{
			dynamic_cast<Things*>(thing)->set_mPos(-100, 0);
			dynamic_cast<Things*>(thing)->move();
			return 1;
		}
		if(dynamic_cast<Roles*>(role)->get_secondlife()){
			dynamic_cast<Roles*>(role)->set_secondlife(false);
			heartClips.x = 0;
			dynamic_cast<Things*>(thing)->set_mPos(-100, 0);
			dynamic_cast<Things*>(thing)->move();
			return 0;
		}
        return -1;
    }
    return 0;
}





int main( int argc, char* args[] )
{
	vector<LTexture* > thingsonscreen;
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
		return 0;
	}
	if( !loadMedia() )
	{
		printf( "Failed to load media!\n" );
		return 0;
	}	
	bool quit = false;
	SDL_Event e;
	
	int crossover = 200;
	int add = 200;
	int rolespe=1;
	
	
	
	int frame = 0;
	bool con = false;
	int choosenum = 0;
	while( !con )
	{
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
		
		chooseTexture[0]->render(gRenderer, 0, 0, NULL);
		chooseTexture[1]->render(gRenderer, 105 + 230*choosenum, 100, &gChooseClip);
		
		SDL_RenderPresent( gRenderer );
		
		while( SDL_PollEvent( &e ) != 0 )
		{
			if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
				con = true;
			else if( e.type == SDL_KEYDOWN )
			{
				switch( e.key.keysym.sym )
				{
					case SDLK_LEFT:
						if(choosenum != 0)
							choosenum-=1;
						else
							choosenum = 2;
						break;

					case SDLK_RIGHT: case SDLK_SPACE:
						if(choosenum != 2)
							choosenum += 1;
						else
							choosenum = 0;
						break;
					
					default:
						break;
				}
				
			}
			gChooseClip.x = 230*choosenum;
		}
	}
	if(choosenum==1){
		dynamic_cast<Roles*>(roleTexture)->set_secondlife(true);
		lifeheart = new Things();
		dynamic_cast<Things*>(lifeheart)->set_mPos(760, 0);
		if(!lifeheart->load( gRenderer, "img/heart.png" ) ){
			printf( "Failed to load heart!\n" );
		}
		else
		{
			heartClips.x = 40;
			heartClips.y = 0;
			heartClips.w = 40;
			heartClips.h = 40;
		}
	}
	if(dynamic_cast<Background*>(bgTexture)->goodrole(choosenum)==1)
		rolespe=2;
	timer.start();
	while( !quit )
	{
		if(timer.getTicks()>=crossover)
		{
			score.ScoreIncrease();
			crossover += add/rolespe;
			if(score.get_mScore()%100 == 0 && add > 80){
				add -=10;
			}
			if(score.get_mScore()%(8*rolespe) == 0){
				srand( time(NULL) );
				switch(rand()%18){
					case 1:  case 2: {
						LTexture *a = new highobstacles(add);
						if(!a->load( gRenderer, "img/thing_high.png" ) ){
							printf( "Failed to load highthings!\n" );
							break;
						}
						thingsonscreen.push_back(a);
						break;
					}
					case 3:{
						LTexture *c = new bonus(add);
						if(!c->load( gRenderer, "img/thing_bonus.png" ) ){
							printf( "Failed to load bonus!\n" );
							break;
						}
						thingsonscreen.push_back(c);
						break;
					}
					case 4: case 5: case 6: case 7:{
						LTexture *b = new lowobstacles(add);
						if(!b->load( gRenderer, "img/low_flower.png" ) ){
							printf( "Failed to load lowthings!\n" );
							break;
						}
						thingsonscreen.push_back(b);
						break;
					}
					case 8: case 9: case 10: case 11: case 12:{
						LTexture *b = new lowobstacles(add);
						if(!b->load( gRenderer, "img/low_catcus.png" ) ){
							printf( "Failed to load lowthings!\n" );
							break;
						}
						thingsonscreen.push_back(b);
						break;
					}
					case 13: case 14: case 15:{
						LTexture *b = new smallobstacles(add);
						if(!b->load( gRenderer, "img/small.png" ) ){
							printf( "Failed to load lowthings!\n" );
							break;
						}
						thingsonscreen.push_back(b);
						break;
					}
					default:
						break;
				}
			}
			if(score.get_mScore()%200 == 0)
			{
				dynamic_cast<Background*>(bgTexture)->change(choosenum, gBackgroundClip); 
			}

		}
		
		string scorestring = "Score: "+to_string(score.get_mScore());
		
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
			else 
			{
				dynamic_cast<Roles*>(roleTexture)->jump(e);
				dynamic_cast<Roles*>(roleTexture)->move();
			}
		}
		SDL_Rect currentClip;
		if(dynamic_cast<Roles*>(roleTexture)->get_mPosY()!= dynamic_cast<Roles*>(roleTexture)->get_mPosY_ini())
			currentClip = gRoleClips[choosenum][0];
		else
			currentClip = gRoleClips[choosenum][frame/60];
		frame++;
		if(frame%12 == 0)
		{
			dynamic_cast<Roles*>(roleTexture)->move();
			if(thingsonscreen.size()>0){
				for(int i=0; i<thingsonscreen.size(); i++){
					dynamic_cast<Things*>(thingsonscreen[i])->move();	
				}
			}				
			for(int i=0; i<2; i++)
				dynamic_cast<decorations*>(cloudTexture[i])->move();
		}
		if(frame == 240)
			frame = 0;
		
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
		
		bgTexture->render(gRenderer, 0, 0, &gBackgroundClip);
		roleTexture->posrender(gRenderer, &currentClip);
		int i=0;
		int x = thingsonscreen.size();


		while(true){
			if(i>=x)
				break;	
			if(!dynamic_cast<Things*>(thingsonscreen[i])->_posrender(gRenderer, NULL))
			{
				x--;
				thingsonscreen.erase(thingsonscreen.begin()+i);
			}
			else
				i++;
		}
		for(int i=0; i<2; i++)
		{
			if(!dynamic_cast<decorations*>(cloudTexture[i])->_posrender(i, gRenderer, NULL))
				break;
		}
		
		if( !dynamic_cast<Text*>(scoreTexture)->loadFromRenderedText( gRenderer, scorestring.c_str(), textColor, gFont ) )
		{	
			printf( "Unable to render time texture!\n" );
		}
		scoreTexture->render(gRenderer, SCREEN_WIDTH - scoreTexture->getWidth() - 10, 10, NULL);
		if(choosenum==1){
			if(!dynamic_cast<Things*>(lifeheart)->_posrender(gRenderer, &heartClips))
				break;
		} 

		if(thingsonscreen.size()>0){
			for (int i=0; i<thingsonscreen.size(); i++){
				if(colresult(thingsonscreen[i], roleTexture)==1){
					score.add_mScore(100);
				}
				else if(colresult(thingsonscreen[i], roleTexture)==-1){
					quit=1;
					printf("quit\n");
				}
			}
		}
		
		SDL_RenderPresent( gRenderer );
	}
	quit = false;
	while(!quit)
	{
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( gRenderer );
		gameoverTexture->render(gRenderer, 0, 0, NULL);
		string num = to_string(score.get_mScore());
		LTexture **score = new LTexture*[num.size()];
		SDL_Rect *numberClips = new SDL_Rect[num.size()];
		for(int i=0; i<num.size(); i++){
			score[i]= new Things;
			dynamic_cast<Things*>(score[i])->set_mPos(657-(num.size()-i)*30, 455);
			if(!score[i]->load( gRenderer, "img/number.png" ) ){
				printf( "Failed to load number!\n" );
				break;
			}
			else
			{
				numberClips[i].x = (num[i]-48)*30;
				numberClips[i].y = 0;
				numberClips[i].w = 30;
				numberClips[i].h = 45;
			}
			
			dynamic_cast<Things*>(score[i])->_posrender(gRenderer, &numberClips[i]);
		}
	
		SDL_RenderPresent( gRenderer );
	}
	
	return 0;
}
