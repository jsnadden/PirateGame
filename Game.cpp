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

    map0 = new Map("assets/terrain.png", 32, 2);
    map0->LoadMap("assets/map0.txt", 16, 16);

    int playerSize = 32;
    int playerScale = 3;
    int offset = (playerSize * playerScale) / 2;
    player.addComponent<TransformComponent>(400 - offset, 320 - offset, playerSize, playerSize, playerScale);
    player.addComponent<SpriteComponent>("assets/ship.png", true, SpriteComponent::loop);
    player.getComponent<SpriteComponent>().AddAnimation("Idle", 0, 3, 10.0f);
    player.getComponent<SpriteComponent>().AddAnimation("Sail", 1, 3, 10.0f);
    player.getComponent<SpriteComponent>().Play("Idle");
    player.addComponent<ControlComponent>();
    player.addGroup(playerGroup);

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
}

void Game::Update()
{
    manager.refresh();
    manager.update();
    
}

void Game::LateUpdate()
{
    input->UpdatePrevious();

    // COLLISION HANDLING GOES HERE

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
