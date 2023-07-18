#pragma once
#include "Game.hpp"

class TextureManager
{
public:

	static SDL_Texture* loadTexture(const char* filename);
	static void Draw(SDL_Texture* texIn, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
};