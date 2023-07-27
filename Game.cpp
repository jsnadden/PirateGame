#include "Game.hpp"

Game* Game::instance = nullptr;
ECSManager manager;

auto& tiles(manager.getGroup(Game::mapGroup));
auto& players(manager.getGroup(Game::playerGroup));
auto& enemies(manager.getGroup(Game::enemyGroup));
auto& colliders(manager.getGroup(Game::colliderGroup));
auto& projectiles(manager.getGroup(Game::projectileGroup));

Map* map0;

auto& player(manager.addEntity());


Game::Game()
{
    quit = false;

    graphics = Graphics::GetInstance();
    if (!Graphics::HasInitialised())
    {
        quit = true;
    }

    assets = Assets::GetInstance();
    input = Input::GetInstance();
    audio = Audio::GetInstance();
    timer = Timer::GetInstance();

    camera = Camera::GetInstance();
    camera->SetDims(Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT);
    viewRect = SDL_Rect{ 0, 0, Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT };

    map0 = new Map("assets/terrain.png", 32, 2);
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
Game::~Game()
{
    Assets::Release();
    assets = nullptr;

    Graphics::Release();
    graphics = nullptr;

    Audio::Release();
    audio = nullptr;

    Timer::Release();
    timer = nullptr;
}

Game* Game::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Game();
    }

    return instance;
}

void Game::Release()
{
    delete instance;
    instance = nullptr;
}

void Game::EarlyUpdate()
{
    input->Update();

    manager.refresh();
    manager.EarlyUpdate();
}

void Game::Update()
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
        t->getComponent<TileComponent>().SetVisibility(Collision::AABB(viewRect, t->getComponent<TileComponent>().Location()));
    }
    //TODO do this for NPCs etc.
}

void Game::LateUpdate()
{
    manager.LateUpdate();
    
    input->UpdatePrevious();
}

void Game::Render()
{
    graphics->ClearRenderer();

    // DRAW CALLS GO HERE

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

    graphics->Render();
}


void Game::Run()
{
    while (!quit)
    {
        timer->Reset();

        while (SDL_PollEvent(&event) != 0)
        {
            if ( event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // TODO remove this in favour of a menu
        if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        EarlyUpdate();

        Update();
        
        LateUpdate();

        Render();

        // CRUDE FRAME RATE LIMITER
        timer->Update();
        if (timer->DeltaTime() < FRAME_SECS)
        {
            SDL_Delay(1000.f * (FRAME_SECS - timer->DeltaTime()));
        }

        timer->Update();
        
    }
}
