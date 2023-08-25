#pragma once
#include "src/Structs/Polygon.hpp"
#include "src/Structs/AABB.hpp"
#include "src/Managers/Managers.hpp"

struct PolyCollider
{
	Polygon polygon;
	SDL_Color colour;
	bool hit;

	PolyCollider(Polygon poly, SDL_Color col = {0xff, 0x00, 0x00, 0xff})
		: polygon(poly), colour(col), hit(false)
	{}

};

struct AABBCollider
{
	std::vector<AABB> boundingBoxes;
	SDL_Color colour;
	bool hit;

	AABBCollider(AABB box, SDL_Color col = { 0xff, 0x00, 0x00, 0x7f })
		: colour(col), hit(false)
	{
		boundingBoxes.emplace_back(box);
	}
};