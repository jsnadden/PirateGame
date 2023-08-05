#include "UIElement.hpp"

UIElement::UIElement()
{
	graphics = Graphics::GetInstance();
	assets = Assets::GetInstance();
	input = Input::GetInstance();

	active = true;
	visible = true;
}

UIElement::~UIElement()
{
}

bool UIElement::IsActive()
{
	return active;
}

void UIElement::SetActive(bool act)
{
	active = act;
}

bool UIElement::IsVisible()
{
	return visible;
}

void UIElement::SetVisibility(bool vis)
{
	visible = vis;
}
