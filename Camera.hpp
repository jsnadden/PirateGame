#pragma once
#include <SDL.h>
#include "Vector2D.hpp"
#include "Map.hpp"

class Camera
{
public:

	static Camera* GetInstance();
	static void Release();

	int OriginX();
	int OriginY();
	int Width();
	int Height();

	void Follow(Vector2D* pos);
	void SetMap(Map& m);
	void SetDims(int w, int h);

	void Update();

private:

	Camera();
	~Camera();

	static Camera* instance;

	Map* map = nullptr;
	Vector2D* centre;
	SDL_Rect view;
	int mapWidth;
	int mapHeight;

};
