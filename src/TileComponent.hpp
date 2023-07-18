#pragma once
#include "ECS.hpp"
#include "SDL.h"
#include "AssetManager.hpp"

class TileComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, int size, int scale, std::string id)
	{
		texture = Game::assets->GetTexture(id);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = size;

		destRect.x = xPos;
		destRect.y = yPos;
		destRect.w = destRect.h = size * scale;

		position.x = xPos;
		position.y = yPos;
	}

	void update(float dt) override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		/* TODO: only draw tiles which are currently visible/on screen.
				 I'll research how this is typically done, but I guess adding
				 a "bool active" flag would be fine */ 
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}


};
