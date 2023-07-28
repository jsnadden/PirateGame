#include "UIElement.hpp"

UIElement::UIElement()
{
	graphics = Graphics::GetInstance();
	assets = Assets::GetInstance();
	input = Input::GetInstance();
}

UIElement::~UIElement()
{
}
