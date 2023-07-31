#include "Collision.hpp"
//#include "ColliderComponent.hpp"

bool Collision::PointRect(const int x, const int y, const SDL_Rect rect)
{
	return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

bool Collision::AABB(const SDL_Rect rectA, const SDL_Rect rectB)
{
	return (rectA.x + rectA.w >= rectB.x)
		&& (rectB.x + rectB.w >= rectA.x)
		&& (rectA.y + rectA.h >= rectB.y)
		&& (rectB.y + rectB.h >= rectA.y);
		
}

bool Collision::RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const SDL_Rect& target,
Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime)
{
	float Nx = (target.x - rayOrigin.x) / rayDirection.x;
	float Ny = (target.y - rayOrigin.y) / rayDirection.y;
	float Fx = (target.x + target.w - rayOrigin.x) / rayDirection.x;
	float Fy = (target.y + target.h - rayOrigin.y) / rayDirection.y;

	if (std::isnan(Nx) || std::isnan(Ny) || std::isnan(Fx) || std::isnan(Fy)) return false;

	if (Nx > Fx) std::swap(Nx, Fx);
	if (Ny > Fy) std::swap(Ny, Fy);

	if (Nx > Fy || Ny > Fx) return false;

	contactTime = std::max(Nx, Ny);

	float leavingTime = std::min(Fx, Fy);

	if (leavingTime < 0.0f || contactTime > 1.0f) return false;

	contactPoint = rayOrigin + (contactTime * rayDirection);

	// TODO: decide what to do on edge case Nx = Ny i.e. ray enters through a corner
	if (Nx > Ny)
	{
		if (rayDirection.x < 0)
			contactNormal = VEC_RIGHT;
		else
			contactNormal = VEC_LEFT;
	}
	else if (Nx < Ny)
	{
		if (rayDirection.y < 0)
			contactNormal = VEC_DOWN;
		else
			contactNormal = VEC_UP;
	}

	return true;
}

bool Collision::SweptAABB(ColliderComponent& colliderA, ColliderComponent& colliderB, float dt,
	Vector2D& contactPos, Vector2D& contactNormal, float& contactTime)
{
	Vector2D vel = *colliderA.Transform()->GetVelocity();

	if (vel.IsZero()) return false;

	SDL_Rect expandedB;
	expandedB.x = colliderB.Location().x - colliderA.Location().w / 2;
	expandedB.y = colliderB.Location().y - colliderA.Location().h / 2;
	expandedB.w = colliderB.Location().w + colliderA.Location().w;
	expandedB.h = colliderB.Location().h + colliderA.Location().h;


	if (RayRect(colliderA.Centre(), vel * dt, expandedB, contactPos, contactNormal, contactTime))
	{
		if (contactTime <= 1.0f) return true;
	}

	return false;
}