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

    if (input->KeyPressed(SDL_SCANCODE_5))
    {
        for (auto& t : *tiles)
        {
            t->destroy();
            t->delGroup(mapGroup);
        }
    }

    if (input->KeyPressed(SDL_SCANCODE_6))
    {
        for (auto& t : *colliders)
        {
            t->destroy();
            t->delGroup(colliderGroup);
        }
    }

    manager.refresh();
    manager.EarlyUpdate();
}

void TestLevel::Update()
{
    Vector2D playerLastPosition = *player->getComponent<TransformComponent>().GetPosition();

    manager.refresh();
    manager.Update();

    camera->Update();

    // Collision handling
    SDL_Rect playerLoc = player->getComponent<ColliderComponent>().Location();

    
    for (auto& c : *colliders)
    {
        // Should do a coarse collision search, using tree methods or whatever

        SDL_Rect cLoc = c->getComponent<ColliderComponent>().Location();

        if (Collision::AABB(playerLoc, cLoc))
        {
            // Should compute the collision "face" and only "push" in the normal direction.
            player->getComponent<TransformComponent>().SetPosition(playerLastPosition);
        }
    }

    
    // Cull off-screen tiles
    for (auto& t : *tiles)
    {
        t->getComponent<TileComponent>().SetVisibility(Collision::AABB(graphics->ViewRect(), t->getComponent<TileComponent>().Location()));
    }
    

}

void TestLevel::LateUpdate()
{
    manager.refresh();
    manager.LateUpdate();
}

void TestLevel::Render()
{
    for (auto& t : *tiles)
    {
        t->draw();
    }

    for (auto& p : *players)
    {
        p->draw();
    }
}
