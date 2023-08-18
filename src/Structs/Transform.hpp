#pragma once
#include "src/Structs/Vector2D.hpp"

struct Transform
{
	Vector2D position;
	Vector2D scale;
	float rotation;

	Transform(Vector2D pos = Vector2D(0.0f,0.0f), Vector2D sc = Vector2D(1.0f, 1.0f), float rot = 0.0f)
		: position(pos), scale(sc), rotation(rot)
	{}

	Transform(float s)
		: position(VEC_ZERO), scale{s,s}, rotation(0.0f)
	{}

	Vector2D operator*(const Vector2D& v) const
	{
		Vector2D scaled = scale * v;
		return position + Vector2D::RotateVector(scaled, rotation);
	}
};