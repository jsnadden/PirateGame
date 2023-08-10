#pragma once
#include "src/ECS/Components.hpp"
#include "src/Managers/Managers.hpp"
#include "src/Structs/Vector2D.hpp"
#include "src/Structs/DynRect.hpp"

class TransformComponent : public Component
{
private:

	Timer* timer;

	Vector2D position;
	Vector2D centre;
	Vector2D velocity;
	float rotation;
	float scale;
	int width;
	int height;

public:

	TransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	TransformComponent(float x, float y, int w, int h, float s = 1.0f)
	{
		position.x = x;
		position.y = y;
		centre.x = x + static_cast<float>(w) / 2;
		centre.y = y + static_cast<float>(h) / 2;
		width = w;
		height = h;
		scale = s;
	}

	~TransformComponent()
	{
		timer = nullptr;
	}

	void init() override
	{
		velocity.Zero();
		timer = Timer::GetInstance();
	}

	void Update() override
	{
		position.x += velocity.x * timer->DeltaTime();
		position.y += velocity.y * timer->DeltaTime();

		centre.x = position.x + scale * static_cast<float>(width) / 2;
		centre.y = position.y + scale * static_cast<float>(height) / 2;
	}

	void SetPosition(Vector2D pos)
	{
		position = pos;
	}

	void Translate(Vector2D disp)
	{
		position += disp;
	}

	Vector2D* GetPosition()
	{
		return &position;
	}

	Vector2D* Centre()
	{
		return &centre;
	}

	DynRect Location()
	{
		return DynRect(position.x, position.y,
			scale * static_cast<float>(width), scale * static_cast<float>(height),
			velocity.x, velocity.y);
	}

	Vector2D* GetVelocity()
	{
		return &velocity;
	}

	void SetVelocity(Vector2D vel)
	{
		velocity.x = vel.x;
		velocity.y = vel.y;
	}

	void SetRotation(float rot)
	{
		rotation = rot;
	}

	void Rotate(float rot)
	{
		rotation += rot;
	}

	float GetRotation()
	{
		return rotation;
	}

	int GetWidth()
	{
		return width;
	}

	int GetHeight()
	{
		return height;
	}

	void SetScale(float sc)
	{
		scale = sc;
	}

	float GetScale()
	{
		return scale;
	}


};