#pragma once
#include "src/Structs/Polygon.hpp"
#include "src/Structs/AABB.hpp"
#include "src/Managers/Managers.hpp"

struct PolyCollider
{
	Polygon polygon;

	SDL_Color colour;

	PolyCollider(Polygon poly, SDL_Color col = {0xff, 0x00, 0x00, 0xff})
		: polygon(poly), colour(col)
	{}

};

struct AABBCollider
{
	std::vector<AABB> boundingBoxes;

	SDL_Color colour;

	AABBCollider(AABB box, SDL_Color col = { 0xff, 0x00, 0x00, 0x7f })
		: colour(col)
	{
		boundingBoxes.emplace_back(box);
	}
};