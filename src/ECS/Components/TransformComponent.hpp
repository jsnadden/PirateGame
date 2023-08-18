#pragma once
#include "src/Structs/Transform.hpp"
#include "src/Structs/DynRect.hpp"

struct TransformComponent
{
public:
	
	Transform transform;


	TransformComponent() = default;

	TransformComponent(Vector2D pos, Vector2D sc, float rot = 0.0f)
		: transform(pos, sc, 0.0f)
	{
	}

	void Translate(Vector2D disp)
	{
		transform.position += disp;
	}

};