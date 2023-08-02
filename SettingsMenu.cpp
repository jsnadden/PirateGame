#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu()
	: State()
{
	states = States::GetInstance();
	
	elements["title"] = new UIText(Vector2D(400, 200), "SETTINGS", 36);

	elements["setting1"] = new Button(Vector2D(400, 300), "SETTING 1");
	elements["setting2"] = new Button(Vector2D(400, 350), "SETTING 2");
	elements["setting3"] = new Button(Vector2D(400, 400), "SETTING 3");
	elements["setting4"] = new Button(Vector2D(400, 450), "SETTING 4");

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
}

void SettingsMenu::Exit()
{
	exit = true;

	// Clean things up!!
	states = nullptr;
	graphics = nullptr;
	timer = nullptr;
	input = nullptr;
	audio = nullptr;
}

void SettingsMenu::EarlyUpdate()
{
	if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
	{
		Exit();
	}
}

void SettingsMenu::Update()
{
	for (auto& e : elements)
	{
		e.second->Update();
	}

	/*if (((Button*)elements["start"])->TestActivated())
	{
		
	}*/
}

void SettingsMenu::LateUpdate()
{

}

void SettingsMenu::Render()
{
	for (auto& e : elements)
	{
		e.second->Draw();
	}
}
