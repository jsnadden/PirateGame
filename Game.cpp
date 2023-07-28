#include "Game.hpp"

Game* Game::instance = nullptr;

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
    states = States::GetInstance();

    camera = Camera::GetInstance();
    camera->SetDims(Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT);

    InitialState();

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

    Input::Release();
    input = nullptr;

    Camera::Release();
    camera = nullptr;
        
    States::Release();
    states = nullptr;

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

void Game::InitialState()
{
    // Eventually this should start a loading screen type thing
    states->StartState<MainMenu>();
}

void Game::EarlyUpdate()
{
    input->Update();

    quit = states->EarlyUpdate();

    
}

void Game::Update()
{
    
    quit = states->Update();

}

void Game::LateUpdate()
{
    quit = states->LateUpdate();
    
    input->UpdatePrevious();
}

void Game::Render()
{
    graphics->ClearRenderer();
    
    quit = states->Render();

    graphics->Render();
}


void Game::Run()
{
    while (!quit)
    {
        timer->Reset();

        EarlyUpdate();

        Update();
        
        LateUpdate();

        Render();

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // TODO remove this in favour of a menu
        if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        // CRUDE FRAME RATE LIMITER
        timer->Update();
        if (timer->DeltaTime() < FRAME_SECS)
        {
            SDL_Delay(1000.f * (FRAME_SECS - timer->DeltaTime()));
        }

        timer->Update();
        
    }
}
