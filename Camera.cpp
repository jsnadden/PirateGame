#include "Camera.hpp"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
	view.x = 0;
	view.y = 0;
}

Camera::~Camera()
{
	map = nullptr;
}

Camera* Camera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Camera();
	}

	return instance;
}

void Camera::Release()
{
	delete instance;
	instance = nullptr;
}

int Camera::OriginX()
{
	return view.x;
}

int Camera::OriginY()
{
	return view.y;
}

void Camera::Follow(Vector2D* pos)
{
	centre = pos;
}

void Camera::SetMap(Map& m)
{
	map = &m;
	mapWidth = m.mapWidth;
	mapHeight = m.mapHeight;
}

void Camera::SetDims(int w, int h)
{
	view.w = w;
	view.h = h;
}

void Camera::Update()
{
	int centreX = 0;
	int centreY = 0;

	if (centre != nullptr)
	{
		centreX = static_cast<int>(centre->x);
		centreY = static_cast<int>(centre->y);
	}

	view.x = centreX - (view.w / 2);
	view.y = centreY - (view.h / 2);

	if (view.x < 0) view.x = 0;
	if (view.y < 0) view.y = 0;
	if (view.x + view.w > mapWidth) view.x = mapWidth - view.w;
	if (view.y + view.h > mapHeight) view.y = mapHeight - view.h;

}