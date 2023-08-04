#pragma once
#include "UIElement.hpp"

class TextPanel : public UIElement
{
private:

	Vector2D centre;
	int scale;
	SDL_Rect destRect;
	SDL_Texture* panelTexture;

public:

	TextPanel(Vector2D position, std::string imgPath, int s = 1);
	~TextPanel();

	void Update();
	void Draw();

};