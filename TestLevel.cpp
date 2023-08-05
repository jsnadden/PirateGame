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
    states = States::GetInstance();
    Init();
}

void TestLevel::Init()
{
    InitMap();
    InitPlayer();
    InitPauseMenu();

    //graphics->WindowTitle("Test Level");
    graphics->SetBackgroundColour(0xf0, 0xf0, 0xff, 0xff);

    for (auto& p : pausemenu)
    {
        elements["pause" + p.first] = p.second;
    }
}

TestLevel::~TestLevel()
{
	Exit();

    graphics = nullptr;
    timer = nullptr;
    audio = nullptr;
    input = nullptr;
    camera = nullptr;
    states = nullptr;

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

        pausemenu["paused"]->SetVisibility(paused);
        pausemenu["settings"]->SetVisibility(paused);
        pausemenu["quit"]->SetVisibility(paused);
        pausemenu["endgame"]->SetVisibility(paused);
    }

    if(!this->paused)
    {
        manager.refresh();
        manager.EarlyUpdate();
    }

    
}

void TestLevel::Update()
{
    for (auto& e : elements)
	{
		if (e.second->IsVisible()) e.second->Update();
	}

    if (((Button*)pausemenu["settings"])->IsActivated())
    {
        states->StartState<SettingsMenu>();
    }

    if (((Button*)pausemenu["quit"])->IsActivated())
    {
        Exit();
    }

    if (((Button*)pausemenu["endgame"])->IsActivated())
    {
        endGame = true;
    }

    if (!this->IsPaused())
    {
        HandleCollision();

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

void TestLevel::InitMap()
{
    map0 = new Map("assets/terrain.png", 32, 2, &manager);
    map0->LoadMap("assets/map0.txt", 16, 16, colliderGroup, mapGroup);
}

void TestLevel::InitPlayer()
{
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
}






void TestLevel::InitPauseMenu()
{
    pausemenu["paused"] = new UIText(Vector2D(400.0f, 200.0f), "PAUSED", 36);

    pausemenu["settings"] = new Button(Vector2D(400.0f, 350.0f), "SETTINGS");
    ((Button*)pausemenu["settings"])->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    ((Button*)pausemenu["settings"])->SetTextColours(WHITE, LIGHT_GREY, BLACK);

    pausemenu["quit"] = new Button(Vector2D(400.0f, 400.0f), "QUIT TO MAIN MENU");
    ((Button*)pausemenu["quit"])->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    ((Button*)pausemenu["quit"])->SetTextColours(WHITE, LIGHT_GREY, BLACK);

    pausemenu["endgame"] = new Button(Vector2D(400.0f, 450.0f), "QUIT TO DESKTOP");
    ((Button*)pausemenu["endgame"])->SetButtonColours(TRANSPARENT, TRANSPARENT, TRANSPARENT);
    ((Button*)pausemenu["endgame"])->SetTextColours(WHITE, LIGHT_GREY, BLACK);

    for (auto& p : pausemenu)
    {
        p.second->SetVisibility(false);
    }
}

void TestLevel::HandleCollision()
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
}
