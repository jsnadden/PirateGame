#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Managers.hpp"


class ProjectileComponent : public Component
{
private:

	Timer* timer;
	Camera* camera;

	int range = 0;
	int speed = 0;
	int distance = 0;
	TransformComponent* transform;
	Vector2D velocity;

public:

	ProjectileComponent(int rng, Vector2D vel)
	{
		timer = Timer::GetInstance();
		camera = Camera::GetInstance();

		range = rng;
		speed = static_cast<int>(vel.Norm());
		velocity = vel;
	}

	~ProjectileComponent()
	{
		timer = nullptr;
		camera = nullptr;
		transform = nullptr;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();
		transform->SetVelocity(velocity);
		//std::cout << "Projectile launched" << std::endl;
	}

	void Update() override
	{
		distance += speed * timer->DeltaTime();

		if (distance > range)
		{
			entity->destroy();
			//std::cout << "Projectile out of range" << std::endl;
		}
		else if (transform->GetPosition()->x < camera->OriginX() || transform->GetPosition()->x > camera->OriginX() + camera->Width()
			|| transform->GetPosition()->y < camera->OriginY() || transform->GetPosition()->y > camera->OriginY() +camera->Height())
		{
			entity->destroy();
			//std::cout << "Projectile out of bounds" << std::endl;
		}

	}
};