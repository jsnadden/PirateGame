#pragma once
#include "src/Structs/Vector2D.hpp"
#include "src/Structs/DynRect.hpp"

struct TransformComponent
{
private:
	
	Vector2D position{ .0f, .0f };
	Vector2D scale{ 1.0f, 1.0f };
	float rotation = 0;

public:

	TransformComponent() = default;

	TransformComponent(Vector2D pos, Vector2D sc)
		: position(pos), scale(sc), rotation(.0f)
	{
	}

	TransformComponent(Vector2D pos, Vector2D sc, float rot)
		: position(pos), scale(sc), rotation(rot)
	{
	}

	void SetPosition(Vector2D pos)
	{
		position = pos;
	}

	void Translate(Vector2D disp)
	{
		position += disp;
	}

	void SetScale(Vector2D sc)
	{
		scale = sc;
	}

	Vector2D* Position()
	{
		return &position;
	}

	Vector2D* Scale()
	{
		return &scale;
	}

};