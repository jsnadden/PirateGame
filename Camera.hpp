#pragma once
#include <SDL.h>
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Map.hpp"

class Camera
{
private:

	Camera();
	~Camera();

	static Camera* instance;

	Entity* parent = nullptr;
	Map* map = nullptr;
	int centerX = 0;
	int centerY = 0;
	SDL_Rect view;
	int levelW;
	int levelH;

public:

	static Camera* GetInstance();
	static void Release();

	void AddParent(Entity& p);
	void AddMap(Map& m);

	void Update();
};
