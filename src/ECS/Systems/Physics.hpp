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

    static void HandleAABBCollision(entt::registry* reg, float dt)
    {
        for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, AABBCollider>().each())
        {
            for (auto b1 : coll1.boundingBoxes)
            {
                Vector2D cp, cn;
                float s = 0;
                std::vector<std::pair<entt::entity, float>> nearbyColliders;

                AABB movingRect = trans1.transform * b1;
                Vector2D vel = trans1.transform.scale * vel1.Velocity();

                // Find potential collisions, make a list of pairs of the form (tile collider, time til collision)
                for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, AABBCollider>().each())
                {
                    // crude broadphasing
                    if ((trans1.transform.position - trans2.transform.position).NormSquared() < 10000 * trans1.transform.scale.x * trans1.transform.scale.x)
                    {
                        for (auto b2 : coll2.boundingBoxes)
                        {
                            AABB tileRect = trans2.transform * b2;

                            if (SweptAABB(movingRect, vel, tileRect, dt, cp, cn, s))
                            {
                                nearbyColliders.push_back({ ent2, s });
                            }
                        }
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
                    auto coll2 = reg->get<AABBCollider>(c.first);

                    for (auto b2 : coll2.boundingBoxes)
                    {
                        AABB tileRect = trans2.transform * b2;

                        Vector2D contactPoint, contactNormal;
                        float s = 0.0f;

                        if (SweptAABB(movingRect, vel, tileRect, dt, contactPoint, contactNormal, s))
                        {
                            // scale normal component of colliderA's velocity so that it will touch, but not penetrate colliderB
                            float nVel = vel1.Velocity().Dot(contactNormal);
                            float tVel = vel1.Velocity().Dot(contactNormal.Orth());
                            Vector2D correctedVelocity = s * nVel * contactNormal + tVel * contactNormal.Orth();
                            vel1.SetVelocity(correctedVelocity);
                            vel = trans1.transform.scale * vel1.Velocity();
                        }
                    }
                    
                }
            }
        }
    }

    static bool SweptAABB(AABB& colliderA, Vector2D& vel, AABB& colliderB, float dt,
        Vector2D& contactPos, Vector2D& contactNormal, float& contactTime)
    {
        // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
        if (vel.IsZero())
            return false;

        // Expand target rectangle by source dimensions
        AABB expandedB = colliderB + colliderA;

        if (Collision::RayRect(colliderA.Centre(), vel * dt, expandedB, contactNormal, contactTime))
            return (contactTime >= 0.0f && contactTime < 1.0f);
        else
            return false;
    }

    static void HandlePolyCollision(entt::registry* reg, float dt)
    {
        for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, PolyCollider>().each())
        {
            Vector2D cn;
            float s = 0;
            std::vector<std::pair<entt::entity, float>> nearbyColliders;

            Polygon movingPoly = trans1.transform * coll1.polygon;
            Vector2D vel = trans1.transform.scale * vel1.Velocity();

            // Find potential collisions, make a list of pairs of the form (tile collider, time til collision)
            for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, PolyCollider>().each())
            {
                // crude broadphasing
                if ((trans1.transform.position - trans2.transform.position).NormSquared() < 10000 * trans1.transform.scale.x * trans1.transform.scale.x)
                {
                    Polygon tilePoly = trans2.transform * coll2.polygon;

                    if (SweptPoly(movingPoly, vel, tilePoly, dt, cn, s))
                    {
                        nearbyColliders.push_back({ ent2, s });
                        std::cout << "collision normal: " << cn << std::endl;
                    }
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
                auto coll2 = reg->get<PolyCollider>(c.first);

                Polygon tilePoly = trans2.transform * coll2.polygon;

                s = 0.0f;

                if (SweptPoly(movingPoly, vel, tilePoly, dt, cn, s))
                {
                    // scale normal component of movingPoly's velocity so that it will touch, but not penetrate tilePoly
                    float nVel = vel1.Velocity().Dot(cn);
                    float tVel = vel1.Velocity().Dot(cn.Orth());
                    Vector2D correctedVelocity = s * nVel * cn + tVel * cn.Orth();
                    vel1.SetVelocity(correctedVelocity);
                    vel = trans1.transform.scale * vel1.Velocity();
                }

            }
        }
    }

    static bool SweptPoly(Polygon& collider1, Vector2D& vel1, Polygon& collider2, float dt,
        Vector2D& contactNormal, float& contactTime)
    {
        // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
        if (vel1.IsZero())
            return false;

        contactNormal = VEC_ZERO;

        Polygon poly1 = collider1;
        Polygon poly2 = collider2;
        Vector2D vel = vel1;

        float overlap = INFINITY;

        float entryTime = -INFINITY;
        float exitTime = INFINITY;

        for (int pass = 1; pass <= 2; pass++)
        {
            if (pass == 2)
            {
                Polygon poly1 = collider2;

                Polygon poly2 = collider1;

                vel = (-1) * vel1;
            }

            for (int a = 0; a < poly2.vertices.size(); a++)
            {
                int b = (a + 1) % poly2.vertices.size();
                Vector2D faceNormal = (poly2.vertices[b] - poly2.vertices[a]).Orth();
                faceNormal.Normalise();

                // Work out min and max for the projection of p2
                float min_p2 = INFINITY, max_p2 = -INFINITY;
                for (int p = 0; p < poly2.vertices.size(); p++)
                {
                    float q = poly2.vertices[p].Dot(faceNormal);
                    min_p2 = std::min(min_p2, q);
                    max_p2 = std::max(max_p2, q);
                }

                // Work out min and max for the projection of p1
                float min_p1 = INFINITY, max_p1 = -INFINITY;
                for (int p = 0; p < poly1.vertices.size(); p++)
                {
                    float q = poly1.vertices[p].Dot(faceNormal);
                    min_p1 = std::min(min_p1, q);
                    max_p1 = std::max(max_p1, q);
                }

                // Project velocity, invert
                float u = 1.0f / (dt * vel1.Dot(faceNormal));

                float s0 = (max_p2 - min_p1) * u;
                float s1 = (min_p2 - max_p1) * u;
                if (std::isnan(s0) || std::isnan(s1)) return false;

                if (s0 > s1) std::swap(s0, s1);

                // keep earliest exit
                exitTime = std::min(exitTime, s1);

                if (exitTime < 0) return false;

                // keep latest entry, and corresponding normal vector
                if (s0 > entryTime)
                {
                    entryTime = s0;
                    contactNormal = faceNormal;
                }

            }
        }

        if (exitTime < entryTime) return false;

        // If we got here, the objects have collided

        contactTime = entryTime;
        return (contactTime >= 0.0f && contactTime < 1.0f);
    }

   
};