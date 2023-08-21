#include "src/Utility/Collision.hpp"


bool Collision::PointRect(const int x, const int y, const SDL_Rect rect)
{
	return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
}

bool Collision::RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, AABB& target, Vector2D& contactNormal, float& contactTime)
{
	
	contactNormal = { 0, 0 };
	//contactPoint = { 0, 0 };

	// Cache division
	Vector2D invdir = {1.0f / rayDirection.x, 1.0f / rayDirection.y };

	// Calculate intersections with rectangle bounding axes
	Vector2D t_near = (target.Position() - rayOrigin) * invdir;
	Vector2D t_far = (target.Position() + target.Size() - rayOrigin) * invdir;

	if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	// Sort distances
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// Early rejection		
	if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

	// Closest 'time' will be the first contact
	contactTime = std::max(t_near.x, t_near.y);

	// Furthest 'time' is contact on opposite side of target
	float t_hit_far = std::min(t_far.x, t_far.y);

	// Reject if ray direction is pointing away from object
	if (t_hit_far < 0)
		return false;

	// Contact point of collision from parametric line equation
	//contactPoint = rayOrigin + contactTime * rayDirection;

	if (t_near.x > t_near.y)
		if (invdir.x < 0)
			contactNormal = VEC_RIGHT;
		else
			contactNormal = VEC_LEFT;
	else if (t_near.x < t_near.y)
		if (invdir.y < 0)
			contactNormal = VEC_DOWN;
		else
			contactNormal = VEC_UP;

	// Note if t_near == t_far, collision is principly in a diagonal
	// By returning a CN={0,0} even though its
	// considered a hit, the resolver wont change anything.
	return true;
}

