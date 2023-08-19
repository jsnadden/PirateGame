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
            // set up global collision parameters
            bool collTest = false;

            for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, ColliderComponent>().each())
            {
                // crude broadphasing
                if ((trans1.transform.position - trans2.transform.position).NormSquared() < 10000 * trans1.transform.scale.x * trans1.transform.scale.x)
                {
                    coll2.colour = { 0xff, 0x7f, 0x00, 0xff };

                    // set up individual collision parameters
                    float pen;
                    collTest |= CheckSAT(coll1.polygon, trans1, coll2.polygon, trans2, pen);

                    // respond to individual collision
                    // TODO

                }
                else coll2.colour = { 0xff, 0x00, 0x00, 0xff };
            }

            // respond to global colission
            if (collTest) coll1.colour = { 0x00, 0xff, 0x00, 0xff };
            else coll1.colour = { 0xff, 0x00, 0x00, 0xff };
        }
    }

    // TODO: try implementing the following
/*---------------------------------------------------------------------------

"Ron Levine" <ron@dorianresearch.com>
Subject:     Re: [Algorithms] Collisions of moving objects
11/14/2000 02:20 PM

For convex polyhedra moving with uniform rectilinear motion (i.e. constant
velocity, no spin), determining whether or not collision occurs and its
exact time in case collision does occur is really no more difficult than
determining whether or not two stationary convex polyhedra intersect.

The idea uses the separating axis theorem.  Recall that this theorem gives
you a finite set of axes such that if the projections of the two bodies onto
on every axis of the set intersect, then you know that the two bodies
intersect in space.  In other words, if the two bodies are separated in
space, then their two projections onto at least one of the separating axes
are separated.

The algorithm goes like this.  You work with the relative velocity vector of
the two convex bodies.  Projecting each of the two bodies and the relative
velocity vector onto a particular separating axis at t0 gives two 1-D
intervals and a 1-D velocity, such that it is easy to tell whether the two
intervals intersect, and if not,  whether they are moving apart or moving
together.  If they are separated and moving apart on any of the separating
axes (or, in fact, on any axis whatever), then you know that there is no
future collision.  If on any separating axis the two projected intervals
intersect at t0 or are separated and are moving together, then it is easy to
compute (by two  simple 1D linear expressions) the earliest future time at
which the two intervals will first intersect and (assuming continuing
rectilinear motion) the latest future time at which the two intervals will
last intersect and begin moving apart.  (If they are intersecting at t0 then
the earliest future intersection time is t0).  Do this for at most all the
separating axes.  If the maximum over all the axes of the earliest future
intersection time is less than the minimum over all the axes of the latest
future intersection time then that maximum earliest future intersection time
is the exact time of first collision of the two 3D polyhedra, otherwise
there is no collision in the future.

The algorithm has the possibility of early outs as you loop over the
separating axes, for if ever the maximum earliest future intersection time
for the axes tested so far is greater than the minimum latest future
intersection time for the axes tested so far, then you know there is no
future collision and are done.


---------------------------------------------------------------------------

Ron Levine <ron@dorianresearch.com>
Subject:     Re: [Algorithms] collision detection and movement
05/04/2002 01:06 PM

Continuing my previous post, here is the elegant solution for finding
the exact time of collision of two axis-aligned boxes in uniform
linear 3D motion.  Actually the method also works for non-axis-aligned
boxes (OBBs) and even for arbitrary convex polyhedra, when you combine
it with the method of separatig axes.  But not to assume you know the
method of separating axes, and to keep the elucidation of the idea
simple, I'll describe the restricted version that applies to the case
of axis-aligned boxes.

Suppose your two boxes are given at time t = 0 by
([x1min, x1max], [y1min, y1max], [z1min,z1max]) and
([x2min, x2max], [y2min, y2max], [z2min,z2max])

and suppose that V1 and V2 are their respective 3D velocity vectors,

Let W = V2 - V1 be their relative velocity vector.

The overview of the algorithm goes like this: We treat the three axes
(x,y,z) in turn.  At any time, the projection of each box onto an axis
is an interval of constant length that is moving uniformly in time
along the axis, and the relative speed of the two projection intervals
is the projection of W onto that axis, thus Wx for the x-axis, etc.
Note that the two projection intervals on one axis can overlap without
the boxes intersecting in 3D space.  In fact, the boxes intersect in
3D space only when the projection intervals overlap SIMULTANEOUSLY on
all three axes. From this consideration, the validity of the following
algorithm should be clear:

For each axis, find the earliest future time (including possibly the
present) t >= 0   such that the two moving projection intervals
overlap and find the latest future time at which they overlap,
assuming continued constant relative speed. (see below)  If on any
axis there is no future time at which the projection intervals
overlap, then there is no future collision.  Now consider the maximum
over all three axes of the earliest future overlap time and the
minimum over all three axes of the latest future overlap time.  If the
maximum earliest future overlap time is greater than the minimum
latest future overlap time, then there is no collision in the future.
If the maximum earliest future overlap time is less than or equal to
the minimum latest future overlap time, then that maximum earliest
future overlap time is the exact time of first collision.

Now, let's look in a little more detail at the determination of
earliest future overlap time and latest future overlap time on the
x-axis.

First suppose that Wx > 0

If x1max < x2min, then the projection of box 2 lies to the right of
the projection of box2 and is moving further to the right, so there is
no future overlap time and so no future collision of the 3D boxes.

If x1min <= x2min <= x1max  or x2min < x1min <= x2max
then the boxes are overlapping at t=0 so the earliest future (or
present) overlap time is 0. But box 2 is still moving to the right
(with respect to box1) and the projection intervals come apart at time
(x1max -- x2min)/Wx, so that is the latest future overlap time.

If x2max < x1min, then the earliest future overlap time is
(x1min - x2max)/Wx and the latest future overlap time is
again (x1max - x2min)/Wx

Now suppose that Wx < 0.  Then you use exactly the same procedure, but
with the roles of box1 and box2 reversed, to get the earliest and
latest overlap times.

If Wx = 0, then we see a divide by zero problem.  In this case there
is no relative motion on the x-axis so it comes down to whether the
two intervals [x1min, x1max] and [x2min, x2max] overlap.  If yes then
both the earilest and latest future overlap times are 0. If no, then
there is no future intersection of the 3D boxes.

QED.

For OBBs, according to the separating axis theorem, there are 15 axes
to check instead of three, and the per axis computation is pretty much
the same except that you have to use some dot products to get the
projections. As a practical matter, you get accurate enough results
for game situations by checking only six of the 15  axes that you need
for an exact result.   And similarly, it extends to arbitrary convex
polyhedra, with the number of axes to be checked growing with the
number of faces and edges of the polyhedra.

Note that this algorithm has frequent early outs in the case of
non-collision, so if you know something about the distribution of
positions and velocities of your colliding bodies, you can speed it up
by appropriately ordering the separating axes.

I discovered this very elegant algorithm for exact collision time and
used it with OBBs in a published game about five years ago, but my
client at the time would not allow me to publiciZe it (competitive
advantage, non-disclosure agreements, etc.)  I did describe it tersely
in a post to this list a couple of years ago.  No one seemed to notice
or appreciate it except Dave Eberly. You can find code implementing it
on his web site, www.magic-software.com.

---------------------------------------------------------------------------*/

    static bool CheckSAT(Polygon& p1, TransformComponent& t1, Polygon& p2, TransformComponent& t2, float& penetration)
    {
        Polygon* poly1 = &p1;
        Transform trans1 = t1.transform;

        Polygon* poly2 = &p2;
        Transform trans2 = t2.transform;

        float overlap = INFINITY;
        penetration = 0;

        for (int shape = 0; shape < 2; shape++)
        {
            if (shape == 1)
            {
                poly1 = &p2;
                trans1 = t2.transform;

                poly2 = &p1;
                trans2 = t1.transform;
            }

            for (int a = 0; a < poly1->vertices.size(); a++)
            {
                int b = (a + 1) % poly1->vertices.size();
                Vector2D axisProj = (trans1 * poly1->vertices[b] - trans1 * poly1->vertices[a]).Orth();
                axisProj.Normalise();

                // Work out min and max 1D points for p1
                float min_p1 = INFINITY, max_p1 = -INFINITY;
                for (int p = 0; p < poly1->vertices.size(); p++)
                {
                    float q = (trans1 * poly1->vertices[p]).Dot(axisProj);
                    min_p1 = std::min(min_p1, q);
                    max_p1 = std::max(max_p1, q);
                }

                // Work out min and max 1D points for r2
                float min_p2 = INFINITY, max_p2 = -INFINITY;
                for (int p = 0; p < poly2->vertices.size(); p++)
                {
                    float q = (trans2 * poly2->vertices[p]).Dot(axisProj);
                    min_p2 = std::min(min_p2, q);
                    max_p2 = std::max(max_p2, q);
                }

                // Calculate actual overlap along projected axis, and store the minimum
                overlap = std::min(std::min(max_p1, max_p2) - std::max(min_p1, min_p2), overlap);

                if (!(max_p2 >= min_p1 && max_p1 >= min_p2))
                {
                    penetration = 0;
                    return false;
                }

                penetration = std::max(penetration, overlap);
            }
        }

        // If we got here, the objects have collided
        return true;
    }


















    //static void HandleMapCollision(entt::registry* reg, float dt)
    //{
    //    for (auto [ent1, trans1, vel1, coll1] : reg->view<TransformComponent, VelocityComponent, ColliderComponent>().each())
    //    {
    //        Vector2D cp, cn;
    //        float s = 0, min_t = INFINITY;
    //        std::vector<std::pair<entt::entity, float>> nearbyColliders;

    //        DynRect movingRect{
    //            trans1.Position()->x + coll1.collider.x * trans1.Scale()->x,
    //            trans1.Position()->y + coll1.collider.y * trans1.Scale()->y,
    //            coll1.collider.w * trans1.Scale()->x,
    //            coll1.collider.h * trans1.Scale()->y,
    //            vel1.Velocity().x,
    //            vel1.Velocity().y
    //        };

    //        // Find potential collisions, make a list of pairs of the form (tile collider, time til collision)
    //        for (auto [ent2, trans2, tile2, coll2] : reg->view<TransformComponent, TileComponent, ColliderComponent>().each())
    //        {
    //            DynRect tileRect{
    //            trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
    //            trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
    //            coll2.collider.w * trans2.Scale()->x,
    //            coll2.collider.h * trans2.Scale()->y, 0, 0 };

    //            if (SweptAABB(movingRect, tileRect, dt, cp, cn, s))
    //            {
    //                nearbyColliders.push_back({ ent2, s });
    //            }
    //        }

    //        // Sort by ascending collision time
    //        std::sort(nearbyColliders.begin(), nearbyColliders.end(), [](const std::pair<entt::entity, float>& a, const std::pair<entt::entity, float>& b)
    //        {
    //            return a.second < b.second;
    //        });

    //        // Handle collisions
    //        for (auto& c : nearbyColliders)
    //        {
    //            auto trans2 = reg->get<TransformComponent>(c.first);
    //            auto coll2 = reg->get<ColliderComponent>(c.first);

    //            DynRect tileRect{
    //            trans2.Position()->x + coll2.collider.x * trans2.Scale()->x,
    //            trans2.Position()->y + coll2.collider.y * trans2.Scale()->y,
    //            coll2.collider.w * trans2.Scale()->x,
    //            coll2.collider.h * trans2.Scale()->y, 0, 0 };

    //            ResolveSweptAABB(movingRect, vel1, tileRect, dt);
    //        }
    //    }
    //}

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