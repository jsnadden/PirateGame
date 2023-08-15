#pragma once
#include <entt.hpp>
#include "src/ECS/Components.hpp"
#include "src/Geometry/Collision.hpp"

class Physics
{
public:

    static void DoMovement(entt::registry* reg, float dt)
    {
        for (auto [entity, trans, vel] : reg->view<TransformComponent, VelocityComponent>().each())
        {
            trans.Translate(dt * vel.Velocity());
        }
    }

    static void HandleMapCollision(entt::registry* reg, float dt)
    {
        for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, ColliderComponent>().each())
        {
            Vector2D cp, cn;
            float s = 0, min_t = INFINITY;
            std::vector<std::pair<entt::entity, float>> nearbyColliders;

            DynRect movingRect{
                trans1.Position()->x + coll1.collider.x * trans1.Scale()->x,
                trans1.Position()->y + coll1.collider.y * trans1.Scale()->y,
                coll1.collider.w * trans1.Scale()->x,
                coll1.collider.h * trans1.Scale()->y,
                vel1.Velocity().x,
                vel1.Velocity().y
            };

            // Find potential collisions, make a list of pairs of the form (tile collider, time til collision)
            for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, ColliderComponent>().each())
            {
                DynRect tileRect{
                trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
                trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
                coll2.collider.w * trans2.Scale()->x,
                coll2.collider.h * trans2.Scale()->y, 0, 0 };

                if (SweptAABB(movingRect, tileRect, dt, cp, cn, s))
                {
                    nearbyColliders.push_back({ ent2, s });
                }
            }

            // Sort by ascending collision time
            std::sort(nearbyColliders.begin(), nearbyColliders.end(), [](const std::pair<entt::entity, float>& a, const std::pair<entt::entity, float>& b)
            {
                return a.second < b.second;
            });

            // Handle collisions
            for (auto& c : nearbyColliders)
            {
                auto trans2 = reg->get<TransformComponent>(c.first);
                auto coll2 = reg->get<ColliderComponent>(c.first);

                DynRect tileRect{
                trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
                trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
                coll2.collider.w * trans2.Scale()->x,
                coll2.collider.h * trans2.Scale()->y, 0, 0 };

                ResolveSweptAABB(movingRect, vel1, tileRect, dt);
            }
        }
    }

    // Check collision for two dynamic, axis-aligned rectangles, passback collision data
    static bool SweptAABB(const DynRect& colliderA, const DynRect& colliderB, float dt,
    	Vector2D& contactPos, Vector2D& contactNormal, float& contactTime)
    {
    	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
    	if (colliderA.vx == 0.0f && colliderA.vy == 0.0f)
    		return false;
    
    	// Expand target rectangle by source dimensions
    	DynRect expandedB = colliderB + colliderA;
    
    	if (Collision::RayRect(colliderA.Centre(), colliderA.GetVelocity() * dt, expandedB, contactPos, contactNormal, contactTime))
    		return (contactTime >= 0.0f && contactTime < 1.0f);
    	else
    		return false;
    }
    
    static bool ResolveSweptAABB(DynRect& colliderA, VelocityComponent& velA, DynRect& colliderB, const float dt)
    {
    	Vector2D contactPoint, contactNormal;
    	float s = 0.0f;
    
    	if (SweptAABB(colliderA, colliderB, dt, contactPoint, contactNormal, s))
    	{
    		// scale normal component of colliderA's velocity so that it will touch, but not penetrate colliderB
    		float nVel = velA.Velocity().Dot(contactNormal);
    		float tVel = velA.Velocity().Dot(contactNormal.Orth());
    		Vector2D correctedVelocity = s * nVel * contactNormal + tVel * contactNormal.Orth();
    		velA.SetVelocity(correctedVelocity);
            colliderA.vx = correctedVelocity.x;
            colliderA.vy = correctedVelocity.y;
    
    		return true;
    	}
    
    	return false;
    }


};