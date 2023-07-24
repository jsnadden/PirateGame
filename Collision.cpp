#include "Collision.hpp"
//#include "ColliderComponent.hpp"

bool Collision::AABB(const SDL_Rect rectA, const SDL_Rect rectB)
{
	return (rectA.x + rectA.w >= rectB.x)
		&& (rectB.x + rectB.w >= rectA.x)
		&& (rectA.y + rectA.h >= rectB.y)
		&& (rectB.y + rectB.h >= rectA.y);
		
} 

//bool Collision::AABB(const ColliderComponent& colliderA, const ColliderComponent& colliderB)
//{
//	if (AABB(colliderA.collider, colliderB.collider))
//	{
//		//std::cout << colliderA.tag << " hit " << colliderB.tag << std::endl;
//		return true;
//	}
//	else
//	{
//		return false;
//	}
// }
