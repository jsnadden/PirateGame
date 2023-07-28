#pragma once
#include "SDL.h"
#include "Managers.hpp"

class TileComponent : public Component
{
private:

	Graphics* graphics;
	Assets* assets;
	Camera* camera;

	SDL_Texture* texture;

	SDL_Rect srcRect, destRect;
	Vector2D worldPosition;

	bool visible;

public:

	TileComponent() = default;

	~TileComponent()
	{
		texture = nullptr;
		graphics = nullptr;
		assets = nullptr;
		camera = nullptr;
	}

	TileComponent(std::string path, int x, int y, int size, Vector2D position, float scale)
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();
		camera = Camera::GetInstance();

		visible = true;

		texture = assets->GetTexture(path);

		srcRect.x = x;
		srcRect.y = y;
		srcRect.w = srcRect.h = size;

		worldPosition = position;
		destRect.x = position.x;
		destRect.y = position.y;
		destRect.w = destRect.h = static_cast<int>(size*scale);
	}

	bool Visible()
	{
		return visible;
	}

	void SetVisibility(bool vis)
	{
		visible = vis;
	}

	SDL_Rect Location()
	{
		return destRect;
	}

	void Update() override
	{
		destRect.x = worldPosition.x - camera->OriginX();
		destRect.y = worldPosition.y - camera->OriginY();
	}

	void draw() override
	{
		if (visible)
		{
			graphics->DrawTexture(texture, &srcRect, &destRect, 0.0f, SDL_FLIP_NONE);
		}
	}


};
