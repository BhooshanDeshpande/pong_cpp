# pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 
#include "utilities.hpp"

class PlayerScore
{ 
	public:
        SDL_Renderer* renderer; 
		TTF_Font* font;
		SDL_Surface* surface = {}; 
		SDL_Texture* texture = {}; 
		SDL_Rect rect = {}; 


		PlayerScore(Vec2 position, SDL_Renderer* renderer, TTF_Font* font);
		~PlayerScore();

		void Draw();
		void SetScore(int score);
}; 