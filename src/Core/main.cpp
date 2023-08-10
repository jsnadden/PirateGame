#include "src/Core/Game.hpp"

int main(int argc, char* argv[])
{
	Game* game = Game::GetInstance();

	game->Run();

	Game::Release();
	game = nullptr;

	return 0;
}