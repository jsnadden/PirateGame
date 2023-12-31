#include "src/Core/Game.hpp"

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	Game* game = Game::GetInstance();

	game->Run();

	Game::Release();
	game = nullptr;

	return 0;
}