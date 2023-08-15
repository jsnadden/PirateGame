#pragma once
#include "src/Structs/DynRect.hpp"
#include "src/Managers/Managers.hpp"

struct ColliderComponent
{
	DynRect collider;
	SDL_Texture* colliderTexture;
	bool visible;

	ColliderComponent(float x, float y, float w, float h)
		: collider{ x, y, w, h, 0.0f, 0.0f }
	{
		colliderTexture = Assets::GetInstance()->GetTexture("assets/sprites/collider.png");
		visible = false;
	}

	~ColliderComponent()
	{
		colliderTexture = nullptr;
	}

};