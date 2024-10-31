
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "LTexture.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

enum roleType { MOON, ORIGINAL, SUN};
enum background { DAY, NIGHT };

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	type = 0;
}

LTexture::~LTexture(){
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		type = 0;
	}
}

bool LTexture::load(SDL_Renderer* gRenderer, string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xFF, 0xFF, 0xFF ) );
		newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		
			SDL_FreeSurface( loadedSurface );
		}
		
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::set_type(int a){
	type = a;
	return; 
}


int LTexture::get_type(){
	return type;
}


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		type = 0;
	}
}


void LTexture::render( SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	//Set clip rendering dimensions

	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

void LTexture::posrender(SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	render(gRenderer, 0, 0, clip);
	return;
}

bool LTexture::_posrender(SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	render(gRenderer, 0, 0, clip);
	return true;
}


int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}

void LTexture::set_mTexture(SDL_Texture* texture)
{
	mTexture = texture;
	return;
}


void LTexture::setWidth(int w)
{
	mWidth = w;
	return;
}

void LTexture::setHeight(int h)
{
	mHeight = h;
	return;
}

Background::Background()
	:LTexture() { }



//void Background::set_type(int a)
//{
//	srand( time(NULL) );
//	LTexture::set_type(rand()%6);
//	return;
//}

int Background::goodrole(int roletype)
{
	int myType = LTexture::get_type();
	switch(roletype)
	{
		case MOON:
			if (myType == NIGHT)
				return 1;
			else
				break;
		case SUN:
			if (myType == DAY)
				return 1;
			else
				break;
		default:
			return 0;
	}
	return 0;
}

int Background::change(int roletype, SDL_Rect& clip)
{
	int b;
	srand(time(NULL));
	b=rand()%2;
	clip.x = b*960;
	LTexture::set_type(b);
	return goodrole(roletype);
}


Text::Text()
	:LTexture() {}



bool Text::loadFromRenderedText(SDL_Renderer* gRenderer, std::string textureText, SDL_Color textColor , TTF_Font* gFont)
{

	LTexture::free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
		LTexture::set_mTexture(SDL_CreateTextureFromSurface( gRenderer, textSurface ));
		if( LTexture::get_mTexture() == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			LTexture::setWidth(textSurface->w);
			LTexture::setHeight(textSurface->h);
		}

		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	return LTexture::get_mTexture() != NULL;
}



Roles::Roles()
	:LTexture()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = mPosY_ini = 490 - ROLES_HEIGHT;
	LTexture::setHeight(ROLES_HEIGHT);
	LTexture::setWidth(ROLES_HEIGHT);
    //Initialize the velocity
    mVelY = 0;
    jumpcount = 0;
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = ROLES_WIDTH;
 	mCollider.h = ROLES_HEIGHT;
}

void Roles::jump( SDL_Event& e )
{
	//If a key was pressed
    if(jumpcount==1)
		return;
	if( e.type == SDL_KEYDOWN && e.key.keysym.sym== SDLK_UP && e.key.repeat == 0 )
    {
		mVelY = -ROLES_VEL;
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.keysym.sym== SDLK_UP && e.key.repeat == 0 )
    {
		mVelY = ROLES_VEL;
        jumpcount=1;
    }
    return;
}

void Roles::move()
{
    mPosY += mVelY;
	if( mPosY <= 140 ){
		mPosY = 140;
		mVelY = ROLES_VEL;
		jumpcount = 1;
	}
    if( mPosY + ROLES_HEIGHT >= 490 && jumpcount==1)
    {
        mPosY = 490 - ROLES_HEIGHT;
        mVelY = 0;
        jumpcount = 0;
    }
    mCollider.y = mPosY;
    return;
}

void Roles::posrender(SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	LTexture::render( gRenderer, mPosX, mPosY, clip);
	return;
}

SDL_Rect Roles::get_mCollider()
{
	return mCollider;
}

void Roles::set_secondlife(bool a){
	secondlife= a;
	return;
}

bool Roles::get_secondlife(){
	return secondlife;
}

Things::Things()
	:LTexture()
{
	mPosX = 0;
	mPosY = 0;
	width=30;
	height=45;
	
	mCollider.w = 0;
	mCollider.h = 0;
}

bool Things::_posrender(SDL_Renderer* gRenderer, SDL_Rect* clip)
{
    if( mPosX + getWidth() <= 0){
		free();
		return false;
	}
	LTexture::render( gRenderer, mPosX, mPosY, clip);
	return true;
}


void Things::free()
{
	LTexture::free();
	mPosX = 0;
	mPosY = 0;
	return;
}

void Things::move()
{
    mPosX -= tvel;
 	mCollider.x = mPosX;
 	return;
}

SDL_Rect Things::get_mCollider()
{
	return mCollider;
}


void Things::set_mPos(int a, int b){
	mPosX=a;
	mPosY=b;
	return;
}

decorations::decorations(int i)
	:Things()
{
	width=80;
	height=44;
	mPosY = 50 + 50*(rand()%3);
    mPosX = 500 + 100*(rand()%4+i);
}
void decorations::move(){
	mPosX -= tvel;
	tvel = 1+rand()%5;
 	return;
}

void decorations::setnewpos(int i){
	mPosY = 100 + 50*(rand()%3);
    mPosX = 960 + 75*(rand()%4+i);
	return;
}

bool decorations::_posrender(int i, SDL_Renderer* gRenderer, SDL_Rect* clip)
{
	if( mPosX + width <= 0)
	{
		setnewpos(i);
	}
	Things::_posrender(gRenderer, clip);
	return true;
}


highobstacles::highobstacles(int t){
	width=106;
	height=70;
	LTexture::setHeight(height);
	LTexture::setWidth(width);
	mPosX = 1000;
    mPosY = 320 - height;
    tvel= 10-t/50;
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = width;
	mCollider.h = height;
	LTexture::set_type(0);
}

lowobstacles::lowobstacles(int t){
	width=80;
	height=106;
	LTexture::setHeight(height);
	LTexture::setWidth(width);
	mPosX = 1000;
    mPosY = 480 - height;
    tvel= 10-t/50;
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = width;
	mCollider.h = height;
	LTexture::set_type(1);
}

smallobstacles::smallobstacles(int t){
	width=80;
	height=70;
	LTexture::setHeight(height);
	LTexture::setWidth(width);
	mPosX = 1000;
    mPosY = 480 - height;
    tvel= 10-t/50;
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = width;
	mCollider.h = height;
	LTexture::set_type(1);
}


bonus::bonus(int t){
	width=36;
	height=48;
	LTexture::setHeight(height);
	LTexture::setWidth(width);
	mPosX = 1000;
	mPosY = 360 + 60*(rand()%3)- height;
    tvel= 10-t/50;
    mCollider.x = mPosX;
    mCollider.y = mPosY;
    mCollider.w = width;
	mCollider.h = height;
	LTexture::set_type(2);
}





