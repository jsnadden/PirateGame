#pragma once
#include <SDL.h>
#include <limits>
#include "src/ECS/Components.hpp"

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

	// Check collision for two axis-aligned rectangles
	static bool AABB(const SDL_Rect rectA, const SDL_Rect rectB);

	// Check collision for a ray and an axis-aligned rectangle, passback collision data
	static bool RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection,
		const DynRect& target, Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime);

	// Check collision for two axis-aligned rectangular ColliderComponents, passback collision data
	static bool SweptAABB(const DynRect& colliderA, const DynRect& colliderB, float dt,
		Vector2D& contactPos, Vector2D& contactNormal, float& contactTime);

	static bool ResolveSweptAABB(Entity* colliderA, Entity* colliderB, const float dt);

};