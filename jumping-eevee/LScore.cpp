#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "LScore.h"
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;


LScore::LScore()
{
	mScore = 0;
	mIncreament = 1;
}

void LScore::ScoreIncrease()
{
	mScore += mIncreament;
	return;
}


int LScore::get_mScore()
{
	return mScore;
}

void LScore::add_mScore(int i)
{
	mScore +=i;
	return;
}


