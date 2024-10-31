#include "LTimer.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;


LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;
	mStartTicks = 0;
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    	time = SDL_GetTicks() - mStartTicks;

    return time;
}
