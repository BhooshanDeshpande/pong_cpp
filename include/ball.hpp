# pragma once
#include "constants.hpp"
#include "utilities.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 


class Ball
{ 
	public: 
		Ball(Vec2 position, Vec2 velocity);
        void Draw(SDL_Renderer* renderer);
        void Update(float dt);
        void CollideWithPaddle(Contact const& contact);
        void CollideWithWall(Contact const& contact);
		Vec2 position; 
		Vec2 velocity; 
		SDL_Rect rect = {}; 
};