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

	void Zoom(float z);
	float GetZoom();
		
	void Follow(Vector2D* pos);
	void SetMapSize(int w, int h);
	void SetDims(int w, int h);

	void Update();

private:

	Camera();
	~Camera();

	static Camera* instance;

	Vector2D* centre;
	float zoom = 1.0f;

	SDL_Rect view;


	int mapWidth;
	int mapHeight;

};
