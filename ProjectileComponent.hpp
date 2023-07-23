#pragma once
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"


class ProjectileComponent : public Component
{
public:

	ProjectileComponent(int rng, int spd, Vector2D vel)
	{
		range = rng;
		speed = spd;
		velocity = vel;
	}

	~ProjectileComponent()
	{}

	void init() override
	{
		/*if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}*/
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		//std::cout << "Projectile launched" << std::endl;
	}

	void update(float dt) override
	{
		distance += speed;

		if (distance > range)
		{
			entity->destroy();
			//std::cout << "Projectile out of range" << std::endl;
		}
		else if (transform->position.x < Game::camera.x || transform->position.x > Game::camera.x + Game::camera.w
			|| transform->position.y < Game::camera.y || transform->position.y > Game::camera.y + Game::camera.h)
		{
			entity->destroy();
			//std::cout << "Projectile out of bounds" << std::endl;
		}

	}


private:

	int range = 0;
	int speed = 0;
	int distance = 0;
	TransformComponent* transform;
	Vector2D velocity;
};