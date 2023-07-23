#pragma once
#include "Components.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

class ControlComponent : public Component
{
private:

    Timer* timer;
    Input* input;

    TransformComponent* transform;
    SpriteComponent* sprite;

public:

    ControlComponent()
    {
        timer = Timer::GetInstance();
        input = Input::GetInstance();
    }

    ~ControlComponent()
    {
        timer = nullptr;
        input = nullptr;
        transform = nullptr;
        sprite = nullptr;
    }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
        Vector2D displace;


        if (input->KeyDown(SDL_SCANCODE_W))
        {
            displace += VEC_UP;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_A))
        {
            displace += VEC_LEFT;
        }
        if (input->KeyDown(SDL_SCANCODE_S))
        {
            displace += VEC_DOWN;
        }
        if (input->KeyDown(SDL_SCANCODE_D))
        {
            displace += VEC_RIGHT;
        }

        displace.Normalise();
        displace *= (timer->DeltaTime() * 100.f);

        transform->Translate(displace);
	}

};