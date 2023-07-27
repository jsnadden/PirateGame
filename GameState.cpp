#include "GameState.hpp"
#include "Map.hpp"
#include "ECS.hpp"
#include "Components.hpp"

ECSManager manager;

auto& tiles(manager.getGroup(GameState::mapGroup));
auto& players(manager.getGroup(GameState::playerGroup));
auto& enemies(manager.getGroup(GameState::enemyGroup));
auto& colliders(manager.getGroup(GameState::colliderGroup));
auto& projectiles(manager.getGroup(GameState::projectileGroup));

Map* map0;

auto& player(manager.addEntity());

GameState::GameState()
	: State()
{
    camera = Camera::GetInstance();
    
    map0 = new Map("assets/terrain.png", 32, 2, &manager);
    map0->LoadMap("assets/map0.txt", 16, 16);
    camera->SetMap(*map0);

    int playerSize = 32;
    int playerScale = 2;
    int offset = (playerSize * playerScale) / 2;
    player.addComponent<TransformComponent>(400 - offset, 320 - offset, playerSize, playerSize, playerScale);
    player.addComponent<SpriteComponent>("assets/ship.png", true, SpriteComponent::loop);
    player.getComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player.getComponent<SpriteComponent>().AddAnimation("Sail", 1, 3, 10.0f);
    player.getComponent<SpriteComponent>().Play("Idle");
    player.addComponent<ControlComponent>();
    player.addComponent<ColliderComponent>("player", 400 - offset, 320 - offset, playerSize * playerScale);
    SDL_Color black = { 0x00, 0x00, 0x00, 0xff };
    player.addComponent<UILabelComponent>(50, 50, "JumpRope Games presents...", "assets/arcade_font.ttf", 18, black);
    player.addGroup(playerGroup);
    camera->Follow(player.getComponent<TransformComponent>().Centre());
    
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
    camera = nullptr;
}

void GameState::EarlyUpdate()
{
    manager.refresh();
    manager.EarlyUpdate();
}

void GameState::Update()
{
    
    Vector2D playerLastPosition = *player.getComponent<TransformComponent>().GetPosition();

    manager.Update();

    camera->Update();

    // Collision handling

    SDL_Rect playerLoc = player.getComponent<ColliderComponent>().Location();

    for (auto& c : colliders)
    {
        // Should do a coarse collision search, using tree methods or whatever

        SDL_Rect cLoc = c->getComponent<ColliderComponent>().Location();

        if (Collision::AABB(playerLoc, cLoc))
        {
            // Should compute the collision "face" and only "push" in the normal direction.
            player.getComponent<TransformComponent>().SetPosition(playerLastPosition);
        }
    }
    // Cull off-screen tiles
    for (auto& t : tiles)
    {
        t->getComponent<TileComponent>().SetVisibility(Collision::AABB(Game::viewRect, t->getComponent<TileComponent>().Location()));
    }

}

void GameState::LateUpdate()
{
    manager.LateUpdate();
}

void GameState::Render()
{
    for (auto& t : tiles)
    {
        t->draw();
    }

    for (auto& p : players)
    {
        p->draw();
    }

    for (auto& c : colliders)
    {
        c->draw();
    }


}
