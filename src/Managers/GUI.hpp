#pragma once
#include "src/GUI/UIElement.hpp"
#include "src/GUI/Button.hpp"
#include "src/GUI/UIText.hpp"

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

