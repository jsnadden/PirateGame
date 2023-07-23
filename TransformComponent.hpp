#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Timer.hpp"

class TransformComponent : public Component
{
private:

	Timer* timer;

	Vector2D position;
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

	//TransformComponent(float xIn, float yIn)
	//{
	//	position.x = xIn;
	//	position.y = yIn;
	//}

	//TransformComponent(int scaleIn)
	//{
	//	// Place the object in the screen center
	//	position.x = 400 - (width * scaleIn / 2);
	//	position.y = 320 - (height * scaleIn / 2);
	//	scale = scaleIn;
	//}

	TransformComponent(float x, float y, int w, int h, float s = 1.0f)
	{
		position.x = x;
		position.y = y;
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

	void update() override
	{
		position.x += velocity.x * timer->DeltaTime();
		position.y += velocity.y * timer->DeltaTime();
	}

	void SetPosition(Vector2D pos)
	{
		position = pos;
	}

	void Translate(Vector2D disp)
	{
		position += disp;
	}

	Vector2D GetPosition()
	{
		return position;
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