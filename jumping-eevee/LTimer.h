#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;


class LTimer
{
    public:
		//Initializes variables
		LTimer();
		void start();
		void stop();
		Uint32 getTicks();

    private:
		Uint32 mStartTicks;
		bool mStarted;
};

