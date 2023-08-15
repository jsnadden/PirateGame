#pragma once
#include "src/Managers/Managers.hpp"
#include "SDL.h"

struct TileComponent
{
public:

	SDL_Texture* texture;

	SDL_Rect srcRect;

	bool visible;

	TileComponent() = default;

	~TileComponent()
	{
		texture = nullptr;
	}

	TileComponent(std::string path, int x, int y, int size)
	{
		visible = true;

		texture = Assets::GetInstance()->GetTexture(path);

		srcRect.x = x;
		srcRect.y = y;
		srcRect.w = srcRect.h = size;
	}

	void SetVisibility(bool vis)
	{
		visible = vis;
	}

};
