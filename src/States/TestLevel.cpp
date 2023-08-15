#include "src/States/TestLevel.hpp"

TestLevel::TestLevel()
	: State()
{
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

    
    if (!this->paused)
    {
        InputHandling::ControlMovement(&enttReg);

        // RUN AI
    }

    
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
    else // if not paused
    {
        for (auto& e : elements)
        {
            if (e.second->IsActive()) e.second->Update();
        }

        
        Physics::HandleMapCollision(&enttReg, timer->DeltaTime());
        Physics::DoMovement(&enttReg, timer->DeltaTime());

        camera->Update();

        // Cull off-screen tiles
        /*for (auto& t : *tiles)
        {
            t->getComponent<TileComponent>().SetVisibility(Collision::AABB(*graphics->ViewRect(), t->getComponent<TileComponent>().Location()));
        }*/

        Rendering::AnimateSprites(&enttReg, timer->DeltaTime());

    }

}

void TestLevel::LateUpdate()
{
    if (!this->IsPaused())
    {
        // ???
    }
    
}

void TestLevel::Render()
{
    // DRAW CALLS

    Rendering::DrawMapTiles(&enttReg);
    Rendering::DrawSprites(&enttReg);
    Rendering::DrawColliders(&enttReg);

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
    map0 = new Map("assets/tiles/terrain.png", 32, 3, &enttReg);
    map0->LoadMap("assets/maps/map0.txt", 16, 16);
}

void TestLevel::InitPlayer()
{
    int playerSize = 32;
    int playerScale = 3;
    int offset = (playerSize * playerScale) / 2;

    player = Factory::CreateEntity(&enttReg, "player");
    player.GetComponent<TransformComponent>().SetPosition(Vector2D(400 - offset, 320 - offset));
    player.GetComponent<TransformComponent>().SetScale(Vector2D(playerScale, playerScale));
    player.AddComponent<VelocityComponent>(VEC_ZERO);
    player.AddComponent<SpriteComponent>("assets/Sprites/ship2.png", playerSize, playerSize, true, SpriteComponent::loop);
    player.GetComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player.GetComponent<SpriteComponent>().AddAnimation("Sail", 1, 3, 10.0f);
    player.GetComponent<SpriteComponent>().Play("Idle");
    player.AddComponent<ColliderComponent>(-16,0,32,16);

    camera->Follow(player.GetComponent<TransformComponent>().Position());
}

void TestLevel::InitGui()
{
    pauseMenu = new PauseMenu();
}

