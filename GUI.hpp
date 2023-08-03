#pragma once
#include "UIElement.hpp"
#include "Button.hpp"
#include "UIText.hpp"

class GUI

{
private:

	GUI();
	~GUI();

	static GUI* instance;

	UIElement* focus;

public:

	static GUI* GetInstance();
	static void Release();

	void FocusOn(UIElement* element);
	UIElement* Focus();
	bool HasFocus();

};

