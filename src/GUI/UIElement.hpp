#pragma once
#include "src/Managers/Managers.hpp"

class UIElement
{
protected:

	Graphics* graphics;
	Assets* assets;
	Input* input;

	bool active;
	bool visible;

public:

	UIElement();
	virtual ~UIElement();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool IsActive();
	void SetActive(bool act);

	bool IsVisible();
	void SetVisibility(bool vis);
};

