#include "src/States/State.hpp"

State::State()
{
	exit = false;
	endGame = false;
	paused = false;

	graphics = Graphics::GetInstance();
	timer = Timer::GetInstance();
	audio = Audio::GetInstance();
	input = Input::GetInstance();

}

State::~State()
{

}

const bool& State::HasExited()
{
	return exit;
}

const bool& State::HasEndedGame()
{
	return endGame;
}

bool State::IsPaused()
{
	return paused;
}

void State::TogglePause()
{
	paused = !paused;
}


