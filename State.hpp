#pragma once
#include <vector>
#include <stack>
#include <map>
#include "ECS.hpp"
#include "Graphics.hpp"
#include "Timer.hpp"

class State
{
protected:

	bool exit;

	Graphics* graphics;
	Timer* timer;

	ECSManager ecs;

public:

	State();
	virtual ~State();

	const bool& HasExited();
	virtual void Exit() = 0;

	virtual void EarlyUpdate() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

};

