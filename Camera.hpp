#pragma once
#include <SDL.h>
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"

struct Camera
{
private:

	Entity* parent = nullptr;
	Map* map = nullptr;

public:

	int centerX = 0;
	int centerY = 0;
	SDL_Rect view;
	int levelW;
	int levelH;

	Camera()
	{
		view.x = 0;
		view.y = 0;
		levelW = view.w = 1;
		levelH = view.h = 1;
	}

	Camera(int w, int h)
	{
		view.x = 0;
		view.y = 0;
		levelW = view.w = w;
		levelH = view.h = h;
	}

	void Parent(Entity& p)
	{
		parent = &p;

		if (!parent->hasComponent<TransformComponent>())
		{
			parent->addComponent<TransformComponent>();
		}
	}

	void Map(Map& m)
	{
		map = &m;
	}

	void Update()
	{
		if (parent != nullptr)
		{
			Vector2D pos = parent->getComponent<TransformComponent>().GetPosition();
			centerX = pos.x;
			centerX = pos.y;
		}
		
		view.x = centerX - (view.w / 2);
		view.y = centerY - (view.h / 2);

		if (view.x < 0) view.x = 0;
		if (view.y < 0) view.y = 0;
		if (view.x + view.w > levelW) view.x = levelW - view.w;
		if (view.y + view.h > levelH) view.y = levelH - view.h;
	}
};
