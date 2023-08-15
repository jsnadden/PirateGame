#pragma once
#include "src/ECS/Entity.hpp"
#include "src/ECS/Components.hpp"

class Factory
{
public:

	static Entity CreateEntity(entt::registry* reg, std::string tag)
    {
        Entity ent = { reg, reg->create() };
        ent.AddComponent<TagComponent>(tag);
        ent.AddComponent<TransformComponent>();
        return ent;
    }

};