#include "State.hpp"

State::State()
{
	exit = false;
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

bool State::IsPaused()
{
	return paused;
}

void State::TogglePause()
{
	paused = !paused;
}


