#pragma once
#include "ECS.hpp"
#include "SDL.h"
#include "Assets.hpp"
#include "Graphics.hpp"

class TileComponent : public Component
{
private:

	Graphics* graphics;
	Assets* assets;

	SDL_Texture* texture;

	SDL_Rect srcRect, destRect;
	Vector2D worldPosition;

	bool active;

public:

	TileComponent() = default;

	~TileComponent()
	{
		texture = nullptr;
		graphics = nullptr;
		assets = nullptr;
	}

	TileComponent(std::string path, int x, int y, int size, Vector2D position, float scale)
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		active = true;

		texture = assets->GetTexture(path);

		srcRect.x = x;
		srcRect.y = y;
		srcRect.w = srcRect.h = size;

		worldPosition = position;
		destRect.x = position.x;
		destRect.y = position.y;
		destRect.w = destRect.h = static_cast<int>(size*scale);
	}

	bool Active()
	{
		return active;
	}

	void Activate()
	{
		active = true;
	}

	void Deactivate()
	{
		active = false;
	}

	void Update() override
	{
		// TODO: subtract camera position, and toggle active if it enters/leaves view
		destRect.x = worldPosition.x;
		destRect.y = worldPosition.y;
	}

	void draw() override
	{
		if (active)
		{
			graphics->DrawTexture(texture, &srcRect, &destRect, 0.0f, SDL_FLIP_NONE);
		}
	}


};
