#include "src/States/TestLevel.hpp"

TestLevel::TestLevel()
	: State()
{
    camera = Camera::GetInstance();
    states = States::GetInstance();

    InitPlayer();
    InitMap();
    InitGui();
    Init();

}

// Init() is called not only when the state is initially created, but also when returning to it,
// so DON'T put things in here that shouldn't happen more than once!!
void TestLevel::Init()
{
    //graphics->WindowTitle("Test Level");
    graphics->SetBackgroundColour(0x00, 0x00, 0x00, 0xff);

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

    delete map;
    map = nullptr;

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

        AI::RunAI(&enttReg, timer->DeltaTime());
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

        
        Physics::HandleAABBCollision(&enttReg, timer->DeltaTime());
        Physics::HandlePolyCollision(&enttReg, timer->DeltaTime());
        Physics::DoMovement(&enttReg, timer->DeltaTime());

        

        Rendering::AnimateSprites(&enttReg, timer->DeltaTime());

    }

}

void TestLevel::LateUpdate()
{
    if (!this->IsPaused())
    {
        ShuffleMaps();
    }

    camera->Update();

    // Cull off-screen tiles
    /*for (auto& t : *tiles)
    {
        t->getComponent<TileComponent>().SetVisibility(Collision::AABB(*graphics->ViewRect(), t->getComponent<TileComponent>().Location()));
    }*/
}

void TestLevel::Render()
{
    // DRAW CALLS

    Rendering::DrawMapTiles(&enttReg);
    Rendering::DrawSprites(&enttReg);
    //Rendering::DrawColliders(&enttReg);

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
    int tileSize = 32;
    int chunkSize = 16;
    chunkPixelSize = tileSize * chunkSize * GLOBAL_SCALE;

    map = new Map(&enttReg, "ocean", "assets/tiles/terrain.png", 2, 2, chunkSize, tileSize, GLOBAL_SCALE, 0, 0);
}

void TestLevel::InitPlayer()
{
    int playerSize = 32;
    int startingPos = (256 - playerSize / 2) * GLOBAL_SCALE;
    int personalSpace = playerSize + 8;

    player = CreateEntity("player");
    player.GetComponent<TransformComponent>().transform.position = Vector2D(startingPos, startingPos);
    player.GetComponent<TransformComponent>().transform.scale = Vector2D(GLOBAL_SCALE, GLOBAL_SCALE);
    player.AddComponent<VelocityComponent>(VEC_ZERO);
    player.AddComponent<SpriteComponent>("assets/Sprites/ship2.png", playerSize, playerSize, true, SpriteComponent::loop);
    player.GetComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player.GetComponent<SpriteComponent>().AddAnimation("Move", 1, 3, 10.0f);
    player.GetComponent<SpriteComponent>().Play("Idle");
    player.AddComponent<AABBCollider>(AABB(-personalSpace / 2, 0, personalSpace, personalSpace /2));
    //player.AddComponent<ControlComponent>();
    player.AddComponent<AIComponent>(1);

    camera->Follow(&player.GetComponent<TransformComponent>().transform.position);
}

void TestLevel::InitGui()
{
    pauseMenu = new PauseMenu();
}

Entity TestLevel::CreateEntity(std::string tag)
{
    Entity ent = { &this->enttReg, enttReg.create() };
    ent.AddComponent<TagComponent>(tag);
    ent.AddComponent<TransformComponent>();
    return ent;
}

void TestLevel::ShuffleMaps()
{
    int chunkX = Maths::divide(camera->CentreX(), chunkPixelSize);
    int chunkY = Maths::divide(camera->CentreY(), chunkPixelSize);

    if (chunkX == lastChunkX - 1) // west
    {
        map->ShuffleWest(chunkX, chunkY);
        std::cout << "exited to west" << std::endl;
        //std::cout << enttReg.alive() << " living entities" << std::endl;
    }

    if (chunkX == lastChunkX + 1) // east
    {
        map->ShuffleEast(chunkX, chunkY);
        std::cout << "exited to east" << std::endl;
        //std::cout << enttReg.alive() << " living entities" << std::endl;
    }

    if (chunkY == lastChunkY - 1) // north
    {
        map->ShuffleNorth(chunkX, chunkY);
        std::cout << "exited to north" << std::endl;
        //std::cout << enttReg.alive() << " living entities" << std::endl;
    }

    if (chunkY == lastChunkY + 1) // south
    {
        map->ShuffleSouth(chunkX, chunkY);
        std::cout << "exited to south" << std::endl;
        //std::cout << enttReg.alive() << " living entities" << std::endl;
    }

    if (std::max(std::abs(lastChunkX - chunkX), std::abs(lastChunkY - chunkY)) > 1)
    {
        map->Teleport(chunkX, chunkY);
    }

    lastChunkX = chunkX;
    lastChunkY = chunkY;
}

