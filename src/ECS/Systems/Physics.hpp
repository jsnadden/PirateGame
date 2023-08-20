#pragma once
#include <entt.hpp>
#include "src/ECS/Components.hpp"
#include "src/Utility/Collision.hpp"

class Physics
{
public:

    static void DoMovement(entt::registry* reg, float dt)
    {
        for (auto [entity, trans, vel] : reg->view<TransformComponent, VelocityComponent>().each())
        {
            trans.Translate(dt * trans.transform.scale * vel.Velocity());
        }
    }

    static void HandleMapCollision(entt::registry* reg, float dt)
    {
        for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, ColliderComponent>().each())
        {
            Vector2D vel = vel1.Velocity();

            std::vector<std::pair<entt::entity, float>> potentialCollisions;

            for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, ColliderComponent>().each())
            {
                // crude broadphasing
                if ((trans1.transform.position - trans2.transform.position).NormSquared() < 10000 * trans1.transform.scale.x * trans1.transform.scale.x)
                {
                    Vector2D n;
                    float s;
                    
                    if (CheckSAT_swept(coll1.polygon, trans1, vel, coll2.polygon, trans2, n, s, dt))
                    {
                        potentialCollisions.push_back({ ent2, s });
                    }

                    if (CheckSAT_static(coll1.polygon, trans1, coll2.polygon, trans2)) coll2.colour = {0x00, 0xff, 0x00, 0xff};
                    else coll2.colour = { 0xff, 0x00, 0x00, 0xff };
                }
            }

            // Sort by ascending collision time
            std::sort(potentialCollisions.begin(), potentialCollisions.end(),
                [](const std::pair<entt::entity, float>& a, const std::pair<entt::entity, float>& b)
                {
                    return a.second < b.second;
                }
            );

            // Handle the collisions in order
            for (auto c : potentialCollisions)
            {
                Polygon poly2 = reg->get<ColliderComponent>(c.first).polygon;
                TransformComponent trans2 = reg->get<TransformComponent>(c.first);
                Vector2D normalVec;
                float s = 0.0f;

                if (CheckSAT_swept(coll1.polygon, trans1, vel, poly2, trans2, normalVec, s, dt))
                {
                    Vector2D tangentVec = normalVec.Orth(); 
                    float velN = vel.Dot(normalVec);
                    float velT = vel.Dot(tangentVec);
                    vel1.SetVelocity(velT * tangentVec + s * velN * normalVec);
                }
            }
        }
    }

    static bool CheckSAT_swept(Polygon& p1, TransformComponent& t1, Vector2D vel1, Polygon& p2, TransformComponent& t2, Vector2D& minNormal, float& s, float dt)
    {
        minNormal = VEC_ZERO;

        if (vel1.IsZero()) return false;

        Polygon* poly1 = &p1;
        Transform trans1 = t1.transform;

        Polygon* poly2 = &p2;
        Transform trans2 = t2.transform;

        float overlap = INFINITY;

        float entryTime = -INFINITY;
        float exitTime = INFINITY;

        for (int a = 0; a < poly2->vertices.size(); a++)
        {
            int b = (a + 1) % poly2->vertices.size();
            Vector2D faceNormal = (trans2 * poly2->vertices[b] - trans2 * poly2->vertices[a]).Orth();
            faceNormal.Normalise();

            // Work out min and max for the projection of p2
            float min_p2 = INFINITY, max_p2 = -INFINITY;
            for (int p = 0; p < poly2->vertices.size(); p++)
            {
                float q = (trans2 * poly2->vertices[p]).Dot(faceNormal);
                min_p2 = std::min(min_p2, q);
                max_p2 = std::max(max_p2, q);
            }

            // Work out min and max for the projection of p1
            float min_p1 = INFINITY, max_p1 = -INFINITY;
            for (int p = 0; p < poly2->vertices.size(); p++)
            {
                float q = (trans1 * poly1->vertices[p]).Dot(faceNormal);
                min_p1 = std::min(min_p1, q);
                max_p1 = std::max(max_p1, q);
            }

            // Project velocity, invert
            float u = 1.0f / (dt * (trans1.scale * vel1).Dot(faceNormal));

            float s0 = (max_p2 - min_p1) * u;
            float s1 = (min_p2 - max_p1) * u;

            if (std::isnan(s0) || std::isnan(s1)) return CheckSAT_static(p1, t1, p2, t2);

            if (s0 > s1) std::swap(s0, s1);

            exitTime = std::min(exitTime, s1);

            if (exitTime < 0) return false;

            if (s0 > entryTime)
            {
                entryTime = s0;
                minNormal = faceNormal;
            }

        }

        if (exitTime < entryTime) return false;

        // If we later implement moving objects with non-AABB colliders, we'll have to do a second pass of the above, with swapped roles

        // If we got here, the objects have collided

        s = entryTime;
        return entryTime >= 0.0f && entryTime < 1.0f;
    }

    static bool CheckSAT_static(Polygon& p1, TransformComponent& t1, Polygon& p2, TransformComponent& t2)
    {
        Polygon* poly1 = &p1;
        Transform trans1 = t1.transform;

        Polygon* poly2 = &p2;
        Transform trans2 = t2.transform;

        float overlap = INFINITY;

        for (int pass = 1; pass <= 2; pass++)
        {
            if (pass == 2)
            {
                Polygon* poly1 = &p2;
                Transform trans1 = t2.transform;

                Polygon* poly2 = &p1;
                Transform trans2 = t1.transform;
            }
            
            for (int a = 0; a < poly2->vertices.size(); a++)
            {
                int b = (a + 1) % poly2->vertices.size();
                Vector2D faceNormal = (trans2 * poly2->vertices[b] - trans2 * poly2->vertices[a]).Orth();
                faceNormal.Normalise();

                // Work out min and max for the projection of p2
                float min_p2 = INFINITY, max_p2 = -INFINITY;
                for (int p = 0; p < poly2->vertices.size(); p++)
                {
                    float q = (trans2 * poly2->vertices[p]).Dot(faceNormal);
                    min_p2 = std::min(min_p2, q);
                    max_p2 = std::max(max_p2, q);
                }

                // Work out min and max for the projection of p1
                float min_p1 = INFINITY, max_p1 = -INFINITY;
                for (int p = 0; p < poly2->vertices.size(); p++)
                {
                    float q = (trans1 * poly1->vertices[p]).Dot(faceNormal);
                    min_p1 = std::min(min_p1, q);
                    max_p1 = std::max(max_p1, q);
                }



                // right now we're just checking if a collision has already taken place
                // need to replace the following with a swept check, and record entrance and exit times
                if (!(max_p1 >= min_p2 && max_p2 >= min_p1)) return false;
                else
                {
                    // Calculate projection overlap, trying to keep the minimum and corresponding normal
                    if ((std::min(max_p1, max_p2) - std::max(min_p1, min_p2)) < overlap)
                    {
                        overlap = (std::min(max_p1, max_p2) - std::max(min_p1, min_p2));
                    }
                }


            }
        }

        // If we got here, the objects have collided
        return true;
    }









    ////////////////////////////////////////////////////////////
    //     aabb collision tests, no longer in use
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

    static void HandleAABB(entt::registry* reg, float dt)
    {
        //for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, ColliderComponent>().each())
        //{
        //    Vector2D cp, cn;
        //    float s = 0, min_t = INFINITY;
        //    std::vector<std::pair<entt::entity, float>> nearbyColliders;

        //    DynRect movingRect{
        //        trans1.Position()->x + coll1.collider.x * trans1.Scale()->x,
        //        trans1.Position()->y + coll1.collider.y * trans1.Scale()->y,
        //        coll1.collider.w * trans1.Scale()->x,
        //        coll1.collider.h * trans1.Scale()->y,
        //        vel1.Velocity().x,
        //        vel1.Velocity().y
        //    };

        //    // Find potential collisions, make a list of pairs of the form (tile collider, time til collision)
        //    for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, ColliderComponent>().each())
        //    {
        //        DynRect tileRect{
        //        trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
        //        trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
        //        coll2.collider.w * trans2.Scale()->x,
        //        coll2.collider.h * trans2.Scale()->y, 0, 0 };

        //        if (SweptAABB(movingRect, tileRect, dt, cp, cn, s))
        //        {
        //            nearbyColliders.push_back({ ent2, s });
        //        }
        //    }

        //    // Sort by ascending collision time
        //    std::sort(nearbyColliders.begin(), nearbyColliders.end(), [](const std::pair<entt::entity, float>& a, const std::pair<entt::entity, float>& b)
        //        {
        //            return a.second < b.second;
        //        });

        //    // Handle collisions
        //    for (auto& c : nearbyColliders)
        //    {
        //        auto trans2 = reg->get<TransformComponent>(c.first);
        //        auto coll2 = reg->get<ColliderComponent>(c.first);

        //        DynRect tileRect{
        //        trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
        //        trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
        //        coll2.collider.w * trans2.Scale()->x,
        //        coll2.collider.h * trans2.Scale()->y, 0, 0 };

        //        ResolveSweptAABB(movingRect, vel1, tileRect, dt);
        //    }
        //}
    }
    //
    ////////////////////////////////////////////////////////////

};