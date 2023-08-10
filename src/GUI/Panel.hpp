#pragma once
#include "src/GUI/UIElement.hpp"

class Panel : public UIElement
{
private:

	Vector2D centre;
	int scale;
	SDL_Rect destRect;
	SDL_Texture* panelTexture;

public:

	Panel(Vector2D position, std::string imgPath, int s = 1);
	~Panel();

	void Update();
	void Draw();

};