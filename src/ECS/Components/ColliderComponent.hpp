#pragma once
#include "src/Structs/DynRect.hpp"
#include "src/Structs/Polygon.hpp"
#include "src/Managers/Managers.hpp"

struct ColliderComponent
{
	Polygon polygon;

	SDL_Color colour;

	ColliderComponent(Polygon poly, SDL_Color col = {0xff, 0x00, 0x00, 0xff})
		: polygon(poly), colour(col)
	{}

	//DynRect collider;
	//SDL_Texture* colliderTexture;

	//ColliderComponent(float x, float y, float w, float h)
	//	: collider{ x, y, w, h, 0.0f, 0.0f }
	//{
	//	colliderTexture = Assets::GetInstance()->GetTexture("assets/sprites/collider.png");
	//}

	//~ColliderComponent()
	//{
	//	colliderTexture = nullptr;
	//}

};