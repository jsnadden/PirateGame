#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char* textureSheet, int xInit, int yInit)
{
	objectTexture = TextureManager::loadTexture(textureSheet);
	xpos = xInit;
	ypos = yInit;
}

void GameObject::Update()
{
	// physics solver
	xpos += ((rand() % 3)-1)*10;
	ypos += ((rand() % 3)-1)*10;

	if (xpos < 0 || xpos > 800 - 64 || ypos < 0 || ypos > 640 - 64) { xpos = 400 - 32; ypos = 300 - 32; }

	srcRect.w = 32;
	srcRect.h = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
	destRect.x = xpos;
	destRect.y = ypos;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objectTexture, &srcRect, &destRect);
}