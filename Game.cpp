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

    graphics->WindowTitle("Untitled Pirate Game");

    assets = Assets::GetInstance();
    input = Input::GetInstance();
    audio = Audio::GetInstance();
    timer = Timer::GetInstance();
    states = States::GetInstance();

    camera = Camera::GetInstance();
    camera->SetDims(Graphics::DEF_WINDOW_WIDTH, Graphics::DEF_WINDOW_HEIGHT);

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
    // For testing/debugging purposes, the following lets me start in any state I'd like
    states->StartState<LogoSplashScreen>();
}

void Game::EarlyUpdate()
{
    input->Update();

    states->EarlyUpdate();

    
}

void Game::Update()
{
    
    states->Update();

}

void Game::LateUpdate()
{
    states->LateUpdate();
    
    input->UpdatePrevious();
}

void Game::Render()
{
    graphics->ClearRenderer();
    
    states->Render();

    graphics->Render();

    quit = states->KillSwitch();
}


void Game::Run()
{
    while (!quit)
    {
        timer->Reset();

        if (!graphics->IsMinimised())
        {
            EarlyUpdate();

            Update();

            LateUpdate();

            Render();
        }
        

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_WINDOWEVENT)
            {
                graphics->HandleWindowEvent(event);
            }
            
        }

        // CRUDE FRAME RATE LIMITER
        timer->Update();
        //std::cout << timer->DeltaTime() << std::endl;
        if (timer->DeltaTime() < FRAME_SECS)
        {
            SDL_Delay(1000.f * (FRAME_SECS - timer->DeltaTime()));
        }
        timer->Update();
        //std::cout << timer->DeltaTime() << std::endl;
    }
}
