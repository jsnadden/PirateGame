#include "src/States/LogoSplashScreen.hpp"

LogoSplashScreen::LogoSplashScreen()
	: State()
{
	states = States::GetInstance();

	elapsedTime = 0.0f;
	alpha = 0;

	SDL_Rect* view = graphics->ViewRect();
	int centreX = view->w / 2;
	int centreY = view->h / 2;

	elements["title1"] = new UIText(Vector2D(centreX, centreY - 150), "JumpRope", 48);
	elements["title2"] = new UIText(Vector2D(centreX, centreY + 150), "Games", 48);
	elements["skippinggirl"] = new UISprite(Vector2D(centreX, centreY), "assets/sprites/jumprope.png", 6, 32, 32, true, 4, 0.09f);

	Init();
}

LogoSplashScreen::~LogoSplashScreen()
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

void LogoSplashScreen::Exit()
{
	exit = true;
}

void LogoSplashScreen::Init()
{
	//graphics->WindowTitle("JumpRope"); 
	graphics->SetBackgroundColour(red, green, blue, 0xff);
}

void LogoSplashScreen::EarlyUpdate()
{

}

void LogoSplashScreen::Update()
{
	for (auto& e : elements)
	{
		e.second->Update();
	}
}

void LogoSplashScreen::LateUpdate()
{
	elapsedTime += timer->DeltaTime();
	alpha = static_cast<Uint8>(255/4 * (elapsedTime - totalTime / 2) * (elapsedTime - totalTime / 2));

	if (input->KeyPressed(SDL_SCANCODE_ESCAPE) || elapsedTime >= totalTime || input->MouseButtonPressed(Input::left))
	{
		states->StartState<MainMenu>();
		elapsedTime = 0.0f;
	}
}

void LogoSplashScreen::Render()
{
	for (auto& e : elements)
	{
		e.second->Draw();
	}

	graphics->FillWindow({ red, green, blue, alpha });
}
