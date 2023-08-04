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
    player->addGroup(playerGroup);
    camera->Follow(player->getComponent<TransformComponent>().Centre());

    elements["paused"] = new UIText(Vector2D(400.0f, 200.0f), "PAUSED", 36);
    elements["quit"] = new Button(Vector2D(400.0f, 350.0f), "QUIT TO MAIN MENU");
    ((Button*)elements["quit"])->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    ((Button*)elements["quit"])->SetTextColours(WHITE, LIGHT_GREY, BLACK);
    
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

    graphics = nullptr;
    timer = nullptr;
    audio = nullptr;
    input = nullptr;
    camera = nullptr;

    delete map0;
    map0 = nullptr;

    for (auto& e : elements)
    {
        delete e.second;
    }
    elements.clear();
}

void TestLevel::Exit()
{
	exit = true;
}

void TestLevel::EarlyUpdate()
{
    if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
    {
        this->TogglePause();
    }

    if (!this->IsPaused())
    {
        elements["paused"]->SetVisibility(false);
        elements["quit"]->SetVisibility(false);

        manager.refresh();
        manager.EarlyUpdate();
    }
    else
    {
        elements["paused"]->SetVisibility(true);
        elements["quit"]->SetVisibility(true);
    }

    
}

void TestLevel::Update()
{
    for (auto& e : elements)
	{
		if (e.second->IsVisible()) e.second->Update();
	}

    if (((Button*)elements["quit"])->IsActivated())
    {
        Exit();
    }

    if (!this->IsPaused())
    {
        player->getComponent<ColliderComponent>().SyncToTransform();

        Vector2D cp, cn;
        float s = 0, min_t = INFINITY;
        std::vector<std::pair<int, float>> nearbyColliders;

        // Find potential collisions, make a list of pairs (time til collision, collider id)
        for (size_t i = 0; i < colliders->size(); i++)
        {
            if (Collision::SweptAABB(player->getComponent<ColliderComponent>().Rectangle(), (*colliders)[i]->getComponent<ColliderComponent>().Rectangle(), timer->DeltaTime(), cp, cn, s))
            {
                nearbyColliders.push_back({ i, s });
            }
        }

        // Sort by ascending collision time
        std::sort(nearbyColliders.begin(), nearbyColliders.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b)
            {
                return a.second < b.second;
            });

        // Handle collisions
        for (auto& c : nearbyColliders)
        {
            Collision::ResolveSweptAABB(player, (*colliders)[c.first], timer->DeltaTime());
        }

        manager.Update();
        camera->Update();

        // Cull off-screen tiles
        for (auto& t : *tiles)
        {
            t->getComponent<TileComponent>().SetVisibility(Collision::AABB(*graphics->ViewRect(), t->getComponent<TileComponent>().Location()));
        }
    }
    
    

}

void TestLevel::LateUpdate()
{
    if (!this->IsPaused())
    {
        manager.LateUpdate();
    }
    
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

    if (this->IsPaused())
    {
        graphics->FillWindow(PAUSE_TINT);
    }

    for (auto& e : elements)
    {
        if (e.second->IsVisible()) e.second->Draw();
    }
    
}
