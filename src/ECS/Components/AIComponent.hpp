#pragma once
#include <entt.hpp>
#include "src/BehaviourTrees/GenericNodes.hpp"
#include "src/ECS/Components.hpp"

struct Behaviours
{
    using Tree = BT::BehaviourTree<float, entt::registry*, entt::entity>;
    using Node = BT::node_ptr<Tree>;

    static Node WaitFor(float duration)
    {
        return BT::Wait<Tree>([=](entt::registry* reg, entt::entity ent)
            {
                return duration;
            });
    }

    static Node CloseTo(entt::entity target, float threshold)
    {
        return BT::Condition<Tree>([=](float dt, entt::registry* reg, entt::entity ent)
            {
                Vector2D myPos = reg->get<TransformComponent>(ent).transform.position;
                Vector2D targetPos = reg->get<TransformComponent>(target).transform.position;
                return (myPos - targetPos).NormSquared() <= threshold * threshold;
            });
    }

    static Node ChooseRandomVelocity(Vector2D* vel)
    {
        return BT::Action<Tree>([=](float dt, entt::registry* reg, entt::entity ent)
            {
                Vector2D myPos = reg->get<TransformComponent>(ent).transform.position;
                float x = (float)(rand() % 200 - 100);
                float y = (float)(rand() % 200 - 100);
                Vector2D randomVel{ x, y };
                randomVel.Normalise();
                randomVel *= 50.0f;
                *vel = randomVel;
            });
    }

    static Node SetVel(Vector2D* vel)
    {
        return BT::Action<Tree>([=](float dt, entt::registry* reg, entt::entity ent)
            {
                reg->get<VelocityComponent>(ent).SetVelocity(*vel);
            });
    }
};

struct AIComponent
{
    Vector2D vel = VEC_ZERO;
    Behaviours::Node root;

    AIComponent(float idleTime)
    {
        root = BT::Sequencer(Behaviours::ChooseRandomVelocity(&vel), Behaviours::WaitFor(idleTime), Behaviours::SetVel(&vel));
    }

    void Tick(float dt, entt::registry* reg, entt::entity ent)
    {
        root->Tick(dt, reg, ent);
        std::cout << vel << std::endl;
    }
};

