#pragma once
#include <vector>
#include <initializer_list>
#include "src/Structs/Vector2D.hpp"

struct Polygon
{
	std::vector<Vector2D> vertices;

	Polygon() = default;

	Polygon(std::initializer_list<Vector2D> pts)
	{
		for (auto p : pts)
		{
			vertices.emplace_back(p);
		}
	}

	Polygon(int n, float r)
	{
		Vector2D p(r, 0);

		for (int i = 0; i < n; i++)
		{
			vertices.emplace_back(Vector2D::RotateVector(p, i * 2 * PI / n));
		}
	}

	Polygon(float x, float y, float w, float h)
	{
		vertices.emplace_back(Vector2D(x, y));
		vertices.emplace_back(Vector2D(x, y + h));
		vertices.emplace_back(Vector2D(x + w, y + h));
		vertices.emplace_back(Vector2D(x + w, y));
	}
};