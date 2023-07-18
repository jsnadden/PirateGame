#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;

	int width = 32;
	int height = 32;
	int scale = 1;

	int speed = 3;

	TransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	TransformComponent(float xIn, float yIn)
	{
		position.x = xIn;
		position.y = yIn;
	}

	TransformComponent(int scaleIn)
	{
		// Place the object in the screen center
		position.x = 400 - (width * scaleIn / 2);
		position.y = 320 - (height * scaleIn / 2);
		scale = scaleIn;
	}

	TransformComponent(float xIn, float yIn, int widthIn, int heightIn, int scaleIn)
	{
		position.x = xIn;
		position.y = yIn;
		width = widthIn;
		height = heightIn;
		scale = scaleIn;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update(float dt) override
	{

		
		
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
		//if (position.x < 0 || position.x > 800 - 64 || position.y < 0 || position.y > 640 - 64) { position.x = (float)(400 - 32); position.y = (float)(300 - 32); }

	}

};