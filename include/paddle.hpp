# pragma once
#include "constants.hpp"
#include "utilities.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 

class Paddle
{ 
	public: 
		Paddle(Vec2 position, Vec2 velocity);
		void Draw(SDL_Renderer* renderer);
		void Update(float dt);

		Vec2 position; 
		Vec2 velocity; 
		SDL_Rect rect = {}; 
};