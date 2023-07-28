#include "TestLevel.hpp"
#include "Map.hpp"
#include "ECS.hpp"
#include "Components.hpp"

ECSManager manager;

auto& tiles(manager.getGroup(TestLevel::mapGroup));
auto& players(manager.getGroup(TestLevel::playerGroup));
auto& enemies(manager.getGroup(TestLevel::enemyGroup));
auto& colliders(manager.getGroup(TestLevel::colliderGroup));
auto& projectiles(manager.getGroup(TestLevel::projectileGroup));

Map* map0;

auto& player(manager.addEntity());

TestLevel::TestLevel()
	: State()
{
    camera = Camera::GetInstance();

    graphics->WindowTitle("Test Level");
    
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
    player.addComponent<UILabelComponent>(350, 600, "TEST LEVEL", "assets/arcade_font.ttf", 18, black);
    player.getComponent<UILabelComponent>().CentreH();
    player.addGroup(playerGroup);
    camera->Follow(player.getComponent<TransformComponent>().Centre());
    
}

TestLevel::~TestLevel()
{
	Exit();
}

void TestLevel::Exit()
{
	exit = true;

	// Clean things up!!
	graphics = nullptr;
	timer = nullptr;
    camera = nullptr;
}

void TestLevel::EarlyUpdate()
{
    manager.refresh();
    manager.EarlyUpdate();
}

void TestLevel::Update()
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
        t->getComponent<TileComponent>().SetVisibility(Collision::AABB(graphics->ViewRect(), t->getComponent<TileComponent>().Location()));
    }

}

void TestLevel::LateUpdate()
{
    manager.LateUpdate();
}

void TestLevel::Render()
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
