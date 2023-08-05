#include "SettingsMenu.hpp"

SettingsMenu::SettingsMenu()
	: State()
{
	states = States::GetInstance();
	
	elements["title"] = new UIText(Vector2D(400, 200), "SETTINGS", 36);

	elements["backbutton"] = new Button(Vector2D(80, 50), "BACK");

	elements["wmpicker"] = new Picker(Vector2D(400, 320), "Window Mode");
	((Picker*)elements["wmpicker"])->AddOption("Windowed");
	((Picker*)elements["wmpicker"])->AddOption("Fullscreen");
	((Picker*)elements["wmpicker"])->SetIndex(graphics->IsFullscreen() ? 1 : 0);


	elements["fsrespicker"] = new Picker(Vector2D(400, 420), "Fullscreen resolution");
	((Picker*)elements["fsrespicker"])->AddOption("option 1");
	((Picker*)elements["fsrespicker"])->AddOption("option 2");
	((Picker*)elements["fsrespicker"])->AddOption("option 3");

	Init();
}

void SettingsMenu::Init()
{
	//graphics->WindowTitle("Settings menu");
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

	if (((Button*)elements["backbutton"])->IsActivated())
	{
		Exit();
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
