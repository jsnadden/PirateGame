#include "LogoSplashScreen.hpp"

LogoSplashScreen::LogoSplashScreen()
	: State()
{
	states = States::GetInstance();

	elapsedTime = 0.0f;
	alpha = 0;

	elements["title1"] = new UIText(Vector2D(400, 170), "JumpRope", 36);
	elements["title2"] = new UIText(Vector2D(400, 470), "Games", 36);
	elements["skippinggirl"] = new UISprite(Vector2D(400, 320), "assets/sprites/jumprope.png", 5, 32, 32, true, 4, 0.09f);

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

	if (input->KeyPressed(SDL_SCANCODE_ESCAPE) || elapsedTime >= totalTime)
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
