#pragma once
#include "Graphics.hpp"

class UIElement
{
protected:

	Graphics* graphics;

public:

	UIElement();
	virtual ~UIElement();

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

