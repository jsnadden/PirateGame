#pragma once
#include "State.hpp"
#include "States.hpp"
#include "MainMenu.hpp"
#include "UISprite.hpp"

class LogoSplashScreen :
    public State
{
private:

    States* states;

    float elapsedTime;
    const float totalTime = 4.0f;

    const Uint8 red = 0x00;
    const Uint8 green = 0x00;
    const Uint8 blue = 0x00;
    Uint8 alpha;

public:

    LogoSplashScreen();
    ~LogoSplashScreen();

    void Exit() override;

    void Init() override;
    void EarlyUpdate() override;
    void Update() override;
    void LateUpdate() override;
    void Render() override;

};

