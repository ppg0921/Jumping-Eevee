
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;


class LScore
{
	public:
		LScore();
		~LScore() {};
		int get_mScore();
		void ScoreIncrease();
		void add_mScore(int);
	private:
		long long mScore;
		int mIncreament;
};
 
