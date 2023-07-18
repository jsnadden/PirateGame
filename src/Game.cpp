#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

int windowWidth = 800;
int windowHeight = 640;
SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());

auto& label(manager.addEntity());

auto& tiles(manager.getGroup(Game::mapGroup));
auto& players(manager.getGroup(Game::playerGroup));
auto& enemies(manager.getGroup(Game::enemyGroup));
auto& colliders(manager.getGroup(Game::colliderGroup));
auto& projectiles(manager.getGroup(Game::projectileGroup));

bool Game::isRunning = false;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	windowWidth = width;
	windowHeight = height;

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	// Initialize SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		isRunning = false;
	}
	else
	{
		// Create window
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			isRunning = false;
		}
		else
		{
			// Create renderer
			Game::renderer = SDL_CreateRenderer(window, -1, 0);
			if (Game::renderer == NULL)
			{
				printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
				isRunning = false;
			}
			else
			{
				// Set background colour to white
				SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);

				if (TTF_Init() == -1)
				{
					printf("SDL_TTF could not initialize! SDL_Error: %s\n", SDL_GetError());
					isRunning = false;
				}
				else
				{
					isRunning = true;

					assets->AddTexture("terrain", "assets/terrain.png");
					assets->AddTexture("player", "assets/ship.png");
					assets->AddTexture("projectile", "assets/cannonball.png");

					assets->AddFont("arcade", "assets/arcade_font.ttf", 16);

					// This is the game content for now:
					map = new Map("terrain", 32, 2);
					map->LoadMap("assets/map0.txt", 16, 16);

					int playerSize = 32;
					int playerScale = 2;
					player.addComponent<TransformComponent>(playerScale);
					player.addComponent<SpriteComponent>("player", true);
					player.addComponent<KeyboardController>();
					player.addComponent <ColliderComponent>("player", 0, 0, playerSize * playerScale);
					player.addGroup(Game::playerGroup);
					SDL_Colour white = { 255, 255, 255, 255 };
					label.addComponent<UILabelComponent>(10, 10, "Yarr!", "arcade", white);


				}
			}
		}
	}
}

void Game::handleEvents()
{
	

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
	default:
		break;
	}
}

void Game::update(float dt)
{
	Vector2D playerLastPosition = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update(dt);

	SDL_Rect playerCollider = player.getComponent<ColliderComponent>().collider;
	TransformComponent* playerTransform = &player.getComponent<TransformComponent>();

	camera.x = playerTransform->position.x - (windowWidth - (playerTransform->width * playerTransform->scale)) / 2;
	camera.y = playerTransform->position.y - (windowHeight - (playerTransform->height * playerTransform->scale)) / 2;
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x + camera.w > 1024) camera.x = 1024 - camera.w;
	if (camera.y + camera.h > 1024) camera.y = 1024 - camera.h;

	// Check for binary collisions:
	for (auto& c : colliders)
	{
		SDL_Rect cCollider = c->getComponent<ColliderComponent>().collider;

		if ( Collision::AABB(playerCollider, cCollider) )
		{
			player.getComponent<TransformComponent>().position = playerLastPosition;
		}
	}

	for (auto& p : projectiles)
	{
		SDL_Rect pCollider = p->getComponent<ColliderComponent>().collider;

		if (Collision::AABB(playerCollider, pCollider))
		{
			//std::cout << "Projectile hit player!" << std::endl;
		}
	}

}

void Game::render()
{
	SDL_RenderClear(Game::renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& e : enemies)
	{
		e->draw();
	}

	for (auto& c : colliders)
	{
		c->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	label.draw();

	// Send to display
	SDL_RenderPresent(Game::renderer);
}

void Game::clean()
{
	// Clean up SDL resources
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(Game::renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}