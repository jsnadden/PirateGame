#include "TextureManager.hpp"

SDL_Texture* TextureManager::loadTexture(const char* filename)
{
	SDL_Surface* tempSurf = IMG_Load(filename);
	SDL_Texture* outTexture = SDL_CreateTextureFromSurface(Game::renderer, tempSurf);
	SDL_FreeSurface(tempSurf);

	return outTexture;
}

void TextureManager::Draw(SDL_Texture* texIn, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Game::renderer, texIn, &srcRect, &destRect, NULL, NULL, flip);
}