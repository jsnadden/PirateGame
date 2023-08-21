#pragma once
#include <SDL.h>
#include <limits>
#include "src/Structs/AABB.hpp"
#include "src/Structs/Vector2D.hpp"

struct Collision
{

	static enum FixedObjects
	{
		neither,
		second,
		first,
		both
	};

	// Check collision for a point and an axis-aligned rectangle
	static bool PointRect(const int x, const int y, const SDL_Rect rect);

	// Check collision for a ray and an axis-aligned rectangle, passback collision data
	static bool RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection,
		AABB& target, Vector2D& contactNormal, float& contactTime);

};