#pragma once

struct VelocityComponent
{
private:

	Vector2D velocity;

public:

	VelocityComponent() = default;

	VelocityComponent(Vector2D vel)
		: velocity(vel)
	{
	}

	Vector2D Velocity()
	{
		return velocity;
	}

	void SetVelocity(Vector2D vel)
	{
		velocity = vel;
	}
};