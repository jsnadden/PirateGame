#pragma once
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Audio.hpp"
#include "Map.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Camera.hpp"
#include "Collision.hpp"

class State
{
private:

	// Add a bunch of manager instances

public:

	State();
	virtual ~State();

	virtual void Update() = 0;
	virtual void Render() = 0;

};

