#pragma once
#include <entt.hpp>
#include "src/ECS/Components.hpp"

class AI
{
public:

	static void RunAI(entt::registry* reg, float dt)
	{
		for (auto [entity, ai, vel] : reg->view<AIComponent, VelocityComponent>().each())
		{
			ai.root->Tick(dt, reg, entity);
		}
	}
};