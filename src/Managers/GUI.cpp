#include "src/Managers/GUI.hpp"

GUI* GUI::instance = nullptr;

GUI::GUI()
{
	focus = nullptr;
}

GUI::~GUI()
{
	focus = nullptr;
}

GUI* GUI::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GUI();
	}

	return instance;
}

void GUI::Release()
{
	delete instance;
	instance = nullptr;
}

void GUI::FocusOn(UIElement* element)
{
	focus = element;
}

UIElement* GUI::Focus()
{
	return focus;
}

bool GUI::HasFocus()
{
	return focus;
}
