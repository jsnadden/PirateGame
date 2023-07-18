#pragma once
#include "Game.hpp"

class GameObject
{
public:

	GameObject(const char* textureSheet, int xInit, int yInit);
	~GameObject();

	void Update();
	void Render();

private:
	
	int xpos;
	int ypos;
	SDL_Texture* objectTexture;
	SDL_Rect srcRect, destRect;
};

