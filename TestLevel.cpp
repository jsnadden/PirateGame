#include "TestLevel.hpp"

TestLevel::TestLevel()
	: State()
{
    tiles = &manager.getGroup(TestLevel::mapGroup);
    players = &manager.getGroup(TestLevel::playerGroup);
    enemies = &manager.getGroup(TestLevel::enemyGroup);
    colliders = &manager.getGroup(TestLevel::colliderGroup);
    projectiles = &manager.getGroup(TestLevel::projectileGroup);

    camera = Camera::GetInstance();

    map0 = new Map("assets/terrain.png", 32, 2, &manager);
    map0->LoadMap("assets/map0.txt", 16, 16, colliderGroup, mapGroup);

    int playerSize = 32;
    int playerScale = 2;
    int offset = (playerSize * playerScale) / 2;
    player = &manager.addEntity();
    player->addComponent<TransformComponent>(400 - offset, 320 - offset, playerSize, playerSize, playerScale);
    player->addComponent<SpriteComponent>("assets/ship.png", true, SpriteComponent::loop);
    player->getComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player->getComponent<SpriteComponent>().AddAnimation("Sail", 1, 3, 10.0f);
    player->getComponent<SpriteComponent>().Play("Idle");
    player->addComponent<ControlComponent>();
    player->addComponent<ColliderComponent>("player", 400 - offset, 320 - offset, playerSize * playerScale);
    player->getComponent<ColliderComponent>().SetRelative(0.0f, 0.5f, 1.0f, 0.5f);
    SDL_Color black = { 0x00, 0x00, 0x00, 0xff };
    player->addComponent<UILabelComponent>(350, 600, "TEST LEVEL", "assets/arcade_font.ttf", 18, black);
    player->getComponent<UILabelComponent>().CentreH();
    player->addGroup(playerGroup);

    camera->Follow(player->getComponent<TransformComponent>().Centre());
    
    Init();
}

void TestLevel::Init()
{
    graphics->WindowTitle("Test Level");
    graphics->SetBackgroundColour(0xf0, 0xf0, 0xff, 0xff);
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
    audio = nullptr;
    input = nullptr;
    camera = nullptr;

    delete map0;
    map0 = nullptr;
}

void TestLevel::EarlyUpdate()
{
    if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
    {
        exit = true;
    }

    manager.refresh();
    manager.EarlyUpdate();
}

void TestLevel::Update()
{
    // Collision handling

    for (auto& c : *colliders)
    {
        // Should do a coarse collision search, using tree methods or whatever

        Vector2D contactPt;
        Vector2D contactNormal;
        float s;

        if (Collision::SweptAABB(player->getComponent<ColliderComponent>(),
            c->getComponent<ColliderComponent>(), timer->DeltaTime(),
            contactPt, contactNormal, s))
        {
            Vector2D vel = *player->getComponent<TransformComponent>().GetVelocity();
            vel += contactNormal * Vector2D(std::abs(vel.x), std::abs(vel.y)) * (1-s);
            player->getComponent<TransformComponent>().SetVelocity(vel);
            c->getComponent<ColliderComponent>().Show();
        }
        
    }

    
    player->getComponent<ColliderComponent>().Show();

    manager.Update();
    camera->Update();

    // Cull off-screen tiles
    for (auto& t : *tiles)
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
    for (auto& t : *tiles)
    {
        t->draw();
    }

    for (auto& c : *colliders)
    {
        c->draw();
    }

    for (auto& p : *players)
    {
        p->draw();
    }

    
}
