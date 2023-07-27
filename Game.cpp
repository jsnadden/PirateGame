#include "Game.hpp"

Game* Game::instance = nullptr;
SDL_Rect Game::viewRect = SDL_Rect{ 0, 0, Graphics::SCREEN_WIDTH, Graphics::SCREEN_HEIGHT };

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

    InitStates();

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

    while (!stateStack.empty())
    {
        stateStack.top()->Exit();
        delete stateStack.top();
        stateStack.pop();
    }
        
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

void Game::InitStates()
{
    stateStack.push(new GameState());
}

void Game::EarlyUpdate()
{
    input->Update();

    if (!stateStack.empty())
    {
        stateStack.top()->EarlyUpdate();
    }

    
}

void Game::Update()
{
    
    if (!stateStack.empty())
    {
        stateStack.top()->Update();

        // Check if current state needs to be ended, quit if no states left.
        if (stateStack.top()->HasExited())
        {
            delete stateStack.top();
            stateStack.pop();
        }
    }
    else
    {
        quit = true;
    }

}

void Game::LateUpdate()
{
    if (!stateStack.empty())
    {
        stateStack.top()->LateUpdate();
    }
    
    input->UpdatePrevious();
}

void Game::Render()
{
    graphics->ClearRenderer();
    
    if (!stateStack.empty())
    {
        stateStack.top()->Render();
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
