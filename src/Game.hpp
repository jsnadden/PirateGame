#pragma once

#include <iostream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.hpp"
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	
	void handleEvents();
	void update(float dt);
	void render();

	void clean();

	static bool isRunning;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;

	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		mapGroup,
		playerGroup,
		enemyGroup,
		colliderGroup,
		projectileGroup
	};

private:
	
	SDL_Window* window;
};