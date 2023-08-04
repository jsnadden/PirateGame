#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu()
	: State()
{
	states = States::GetInstance();
	
	elements["title"] = new UIText(Vector2D(400, 200), "SETTINGS", 36);

	elements["pickertest1"] = new Picker(Vector2D(400, 320), "A setting");
	((Picker*)elements["pickertest1"])->AddOption("option 1");
	((Picker*)elements["pickertest1"])->AddOption("option 2");
	((Picker*)elements["pickertest1"])->AddOption("option 3");

	elements["pickertest2"] = new Picker(Vector2D(400, 420), "Another setting");
	((Picker*)elements["pickertest2"])->AddOption("other option 1");
	((Picker*)elements["pickertest2"])->AddOption("other option 2");
	((Picker*)elements["pickertest2"])->AddOption("other option 3");




	Init();
}

void SettingsMenu::Init()
{
	graphics->WindowTitle("Settings menu");
	graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);
}

SettingsMenu::~SettingsMenu()
{
	Exit();

	states = nullptr;
	graphics = nullptr;
	timer = nullptr;
	input = nullptr;
	audio = nullptr;

	for (auto& e : elements)
	{
		delete e.second;
	}
	elements.clear();
}

void SettingsMenu::Exit()
{
	exit = true;
}

void SettingsMenu::EarlyUpdate()
{
	
}

void SettingsMenu::Update()
{
	for (auto& e : elements)
	{
		e.second->Update();
	}

}

void SettingsMenu::LateUpdate()
{
	if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
	{
		Exit();
	}
}

void SettingsMenu::Render()
{
	for (auto& e : elements)
	{
		e.second->Draw();
	}
}
