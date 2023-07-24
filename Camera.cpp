#include "Camera.hpp"

Camera* Camera::instance = nullptr;

Camera::Camera()
{
	view.x = 0;
	view.y = 0;
}

Camera::~Camera()
{
	parent = nullptr;
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

//TODO: add set/get methods for the position and rectangle stuff

void Camera::AddParent(Entity& p)
{
	parent = &p;

	if (!parent->hasComponent<TransformComponent>())
	{
		parent->addComponent<TransformComponent>();
	}
}

void Camera::AddMap(Map& m)
{
	map = &m;
}

void Camera::Update()
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