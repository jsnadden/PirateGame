#pragma once
#include "Managers.hpp"

class UIElement
{
protected:

	Graphics* graphics;
	Assets* assets;
	Input* input;

public:

	UIElement();
	virtual ~UIElement();

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

