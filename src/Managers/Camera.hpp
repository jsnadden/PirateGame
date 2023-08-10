#pragma once
#include <SDL.h>
#include "src/Structs/Vector2D.hpp"

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
	void SetMapSize(int w, int h);
	void SetDims(int w, int h);

	void Update();

private:

	Camera();
	~Camera();

	static Camera* instance;

	//Map* map = nullptr;
	Vector2D* centre;
	SDL_Rect view;
	int mapWidth;
	int mapHeight;

};
