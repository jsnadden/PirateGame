#include "src/States/TestLevel.hpp"

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

    InitMap();
    InitPlayer();
    InitGui();
    Init();
}

// Init() is called not only when the state is initially created, but also when returning to it,
// so DON'T put things in here that shouldn't happen more than once!!
void TestLevel::Init()
{
    //graphics->WindowTitle("Test Level");
    graphics->SetBackgroundColour(0xf0, 0xf0, 0xff, 0xff);

    pauseMenu->SetActive(paused);
    pauseMenu->SetVisibility(paused);
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

    pauseMenu = nullptr;
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

        pauseMenu->SetActive(paused);
        pauseMenu->SetVisibility(paused);
    }

    manager.refresh();
    if(!this->paused) manager.EarlyUpdate();

    
}

void TestLevel::Update()
{
    if (this->IsPaused())
    {
        if (pauseMenu->IsActive()) pauseMenu->Update();

        if (pauseMenu->Pressed("settings"))
        {
            states->StartState<SettingsMenu>();
        }

        if (pauseMenu->Pressed("quit"))
        {
            Exit();
        }

        if (pauseMenu->Pressed("endgame"))
        {
            endGame = true;
        }
    }
    else // i.e. not paused
    {
        for (auto& e : elements)
        {
            if (e.second->IsActive()) e.second->Update();
        }

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

    for (auto& e : elements)
    {
        if (e.second->IsVisible()) e.second->Draw();
    }

    if (this->IsPaused())
    {
        graphics->FillWindow(PAUSE_TINT);

        pauseMenu->Draw();
    }

}

void TestLevel::InitMap()
{
    map0 = new Map("assets/tiles/terrain.png", 32, 3, &manager);
    map0->LoadMap("assets/maps/map0.txt", 16, 16, colliderGroup, mapGroup);
}

void TestLevel::InitPlayer()
{
    int playerSize = 32;
    int playerScale = 3;
    int offset = (playerSize * playerScale) / 2;
    player = &manager.addEntity();
    player->addComponent<TransformComponent>(400 - offset, 320 - offset, playerSize, playerSize, playerScale);
    player->addComponent<SpriteComponent>("assets/sprites/ship2.png", true, SpriteComponent::loop);
    player->getComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player->getComponent<SpriteComponent>().AddAnimation("Sail", 1, 3, 10.0f);
    player->getComponent<SpriteComponent>().Play("Idle");
    player->addComponent<ControlComponent>();
    player->addComponent<ColliderComponent>("player", 400 - offset, 320 - offset, playerSize * playerScale);
    player->getComponent<ColliderComponent>().SetRelative(0.0f, 0.5f, 1.0f, 0.5f);
    player->addGroup(playerGroup);
    camera->Follow(player->getComponent<TransformComponent>().Centre());
}






void TestLevel::InitGui()
{
    pauseMenu = new PauseMenu();
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
