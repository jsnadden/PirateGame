#pragma once
#include "src/Structs/Vector2D.hpp"

struct AABB
{
	float x;
	float y;
	float w;
	float h;

	AABB() = default;

	AABB(float posX, float posY, float width, float height)
	{
		x = posX;
		y = posY;
		w = width;
		h = height;
	}

	Vector2D Position()
	{
		return Vector2D(x, y);
	}

	Vector2D Size()
	{
		return Vector2D(w, h);
	}

	Vector2D Centre()
	{
		return Vector2D(x + 0.5f * w, y + 0.5f * h);
	}

	AABB operator+(const AABB& other) const
	{
		return AABB(this->x - other.w / 2, this->y - other.h / 2, this->w + other.w, this->h + other.h);
	}
};