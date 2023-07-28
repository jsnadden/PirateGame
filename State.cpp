#include "State.hpp"

State::State()
{
	exit = false;

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


