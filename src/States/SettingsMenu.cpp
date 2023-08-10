#include "src/States/SettingsMenu.hpp"

SettingsMenu::SettingsMenu()
	: State()
{
	SDL_Rect* view = graphics->ViewRect();
	int centreX = view->w / 2;
	int centreY = view->h / 2;
	
	elements["title"] = new UIText(Vector2D(centreX, 120), "SETTINGS", 36);

	elements["backbutton"] = new Button(Vector2D(centreX - 200, 600), "BACK");

	elements["confirmbutton"] = new Button(Vector2D(centreX + 200, 600), "APPLY");
	elements["confirmbutton"]->SetActive(false);

	elements["wmpicker"] = new Picker(Vector2D(centreX, centreY), "Window Mode");
	((Picker*)elements["wmpicker"])->AddOption("Windowed");
	((Picker*)elements["wmpicker"])->AddOption("Fullscreen");
	((Picker*)elements["wmpicker"])->SetIndex(graphics->IsFullscreen() ? 1 : 0);
	((Picker*)elements["wmpicker"])->Confirm();

	elements["anothersetting"] = new Picker(Vector2D(centreX, centreY + 100), "Another setting");
	((Picker*)elements["anothersetting"])->AddOption("option 1");
	((Picker*)elements["anothersetting"])->AddOption("option 2");
	((Picker*)elements["anothersetting"])->AddOption("option 3");

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
	// once I have other settings to change, this logic will need to be reworked
	if (((Picker*)elements["wmpicker"])->HasChanged())
	{
		elements["confirmbutton"]->SetActive(true);

		if (((Button*)elements["confirmbutton"])->IsActivated())
		{
			graphics->SetFullScreen(((Picker*)elements["wmpicker"])->Index());
			((Picker*)elements["wmpicker"])->Confirm();
			SaveSettings();
		}
	}
	else
	{
		elements["confirmbutton"]->SetActive(false);
	}
	

	for (auto& e : elements)
	{
		if (e.second->IsActive()) e.second->Update();
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
		if (e.second->IsVisible()) e.second->Draw();
	}
}

void SettingsMenu::SaveSettings()
{
	std::ofstream settings("config/settings.txt");

	if (settings.is_open())
	{
		settings << graphics->IsFullscreen();
	}
	else printf("Config file settings.txt could not be loaded!\n");

	settings.close();
}
