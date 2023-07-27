#include "GameState.hpp"


GameState::GameState()
	: State()
{
	
}

GameState::~GameState()
{
	Exit();
}

void GameState::Exit()
{
	exit = true;

	// Clean things up!!
	graphics = nullptr;
	timer = nullptr;
}

void GameState::EarlyUpdate()
{

}

void GameState::Update()
{
	
}

void GameState::LateUpdate()
{

}

void GameState::Render()
{

}
