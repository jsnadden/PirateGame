#include "src/Physics/Collision.hpp"


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

bool Collision::RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const DynRect& target,
Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime)
{
	
	contactNormal = { 0, 0 };
	contactPoint = { 0, 0 };

	// Cache division
	Vector2D invdir = {1.0f / rayDirection.x, 1.0f / rayDirection.y };

	// Calculate intersections with rectangle bounding axes
	Vector2D t_near = (target.GetPosition() - rayOrigin) * invdir;
	Vector2D t_far = (target.GetPosition() + target.GetSize() - rayOrigin) * invdir;

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
	contactPoint = rayOrigin + contactTime * rayDirection;

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

bool Collision::SweptAABB(const DynRect& colliderA, const DynRect& colliderB, float dt,
	Vector2D& contactPos, Vector2D& contactNormal, float& contactTime)
{
	
	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (colliderA.vx == 0.0f && colliderA.vy == 0.0f)
		return false;

	// Expand target rectangle by source dimensions
	DynRect expandedB = colliderB + colliderA;

	if (RayRect(colliderA.Centre(), colliderA.GetVelocity() * dt, expandedB, contactPos, contactNormal, contactTime))
		return (contactTime >= 0.0f && contactTime < 1.0f);
	else
		return false;

}

bool Collision::ResolveSweptAABB(Entity* colliderA, Entity* colliderB, const float dt)
{
	Vector2D contactPoint, contactNormal;
	float s = 0.0f;

	if (Collision::SweptAABB(colliderA->getComponent<ColliderComponent>().Rectangle(), colliderB->getComponent<ColliderComponent>().Rectangle(), dt,
		contactPoint, contactNormal, s))
	{
		// scale normal component of colliderA's velocity so that it will touch, but not penetrate colliderB
		float nVel = colliderA->getComponent<ColliderComponent>().Rectangle().GetVelocity().Dot(contactNormal);
		float tVel = colliderA->getComponent<ColliderComponent>().Rectangle().GetVelocity().Dot(contactNormal.Orth());
		Vector2D correctedVelocity = s * nVel * contactNormal + tVel * contactNormal.Orth();
		colliderA->getComponent<TransformComponent>().SetVelocity(correctedVelocity);
		colliderA->getComponent<ColliderComponent>().SyncToTransform();
		colliderB->getComponent<ColliderComponent>().Show();

		return true;
	}

	return false;
}