#include "UIElement.hpp"

UIElement::UIElement()
{
	graphics = Graphics::GetInstance();
	assets = Assets::GetInstance();
	input = Input::GetInstance();

	visible = true;
}

UIElement::~UIElement()
{
}

bool UIElement::IsVisible()
{
	return visible;
}

void UIElement::SetVisibility(bool vis)
{
	visible = vis;
}
