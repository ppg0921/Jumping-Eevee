#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

const int EBUSIZE = 100;

class LTexture
{
	public:
		LTexture();
		LTexture(int);
		~LTexture();
		bool load(SDL_Renderer* gRenderer, string);
		virtual void set_type(int a);
		void set_mTexture(SDL_Texture*);
		int get_type();
		int getWidth();
		int getHeight();
		void setWidth(int);
		void setHeight(int);
		virtual void free();
		virtual void render( SDL_Renderer* gRenderer, int x, int y, SDL_Rect* clip);
		virtual void posrender(SDL_Renderer* gRenderer, SDL_Rect* clip = NULL);
		virtual bool _posrender(SDL_Renderer* gRenderer, SDL_Rect* clip = NULL);
		virtual SDL_Rect get_mCollider(){
			SDL_Rect a;
			a.h= a.w= a.x= a.y =0;
			return a; 
		};
		SDL_Texture* get_mTexture(){return mTexture;}

	private:
		SDL_Texture* mTexture;
		int type;
		int mWidth;
		int mHeight;
};





class Background : public LTexture
{
	public:
		Background();
		//void set_type(int);
		int goodrole(int);
		int change(int, SDL_Rect& clip);
	private:
		SDL_Texture* mTexture;
};


class Text : public LTexture
{
	public:
		Text();
		bool loadFromRenderedText( SDL_Renderer* gRenderer, string textureText, SDL_Color textColor, TTF_Font* );
	private:
		SDL_Texture* mTexture;
};



class Roles : public LTexture
{
	public:
		static const int ROLES_WIDTH = EBUSIZE;
		static const int ROLES_HEIGHT = EBUSIZE;
		static const int ROLES_VEL = 4;
		Roles();
		void jump( SDL_Event& e );
		void move();
		void posrender(SDL_Renderer* gRenderer, SDL_Rect* clip);
		SDL_Rect get_mCollider();
		void set_secondlife(bool);
		bool get_secondlife();
		int get_mPosY(){
			return mPosY;
		}
		int get_mPosY_ini(){
			return mPosY_ini;
		}

    private:
  		int mPosX, mPosY;
		int mVelY, mPosY_ini;
  		int jumpcount=0;
  		SDL_Rect mCollider;
		SDL_Texture* mTexture;
		bool secondlife= false;
};



class Things : public LTexture
{
	public:
		Things();
  		virtual void move();
  		virtual bool _posrender(SDL_Renderer* gRenderer, SDL_Rect* clip);
  		void free();
  		SDL_Rect get_mCollider();
  		void set_mPos(int, int);
  	protected:
  		int mPosX, mPosY;
  		int width, height;
  		int tvel =6;
  		SDL_Rect mCollider;
  		SDL_Texture* mTexture;
};

class decorations: public Things{
	public:
		decorations(int);
		void move();
		void setnewpos(int);
		bool _posrender(int, SDL_Renderer* gRenderer, SDL_Rect* clip);
};

class highobstacles: public Things{
	public:
		highobstacles(int);
};

class lowobstacles: public Things{
	public:
		lowobstacles(int);
};

class smallobstacles: public Things{
	public:
		smallobstacles(int);
};

class bonus: public Things{
	public:
		bonus(int);
};




