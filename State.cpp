#include "State.hpp"

State::State()
{
	exit = false;

	graphics = Graphics::GetInstance();
	timer = Timer::GetInstance();
}

State::~State()
{

}

const bool& State::HasExited()
{
	return exit;
}


