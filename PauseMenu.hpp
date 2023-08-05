#pragma once
#include "UIElement.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include "UIText.hpp"

class PauseMenu :
    public UIElement
{
private:

	Panel* backPanel;
	UIText* title;
	std::map<std::string, Button*> buttons;

	SDL_Color BLACK = { 0x00, 0x00, 0x00, 0xff };
	SDL_Color LIGHT_GREY = { 0xa0, 0xa0, 0xa0, 0xff };
	SDL_Color WHITE = { 0xff, 0xff, 0xff, 0xff };
	SDL_Color TRANSPARENT = { 0x00, 0x00, 0x00, 0x00 };

public:

	PauseMenu();
	~PauseMenu();

	void Update();
	void Draw();

	bool Pressed(std::string buttonID);
};

