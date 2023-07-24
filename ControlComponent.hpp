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

	void Update() override
	{
        Vector2D displace;
        sprite->Play("Idle");

        if (input->KeyDown(SDL_SCANCODE_W))
        {
            displace += VEC_UP;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_A))
        {
            displace += VEC_LEFT;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_S))
        {
            displace += VEC_DOWN;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_D))
        {
            displace += VEC_RIGHT;
            sprite->Play("Sail");
        }
        if (input->KeyPressed(SDL_SCANCODE_A))
        {
            sprite->Flip(SDL_FLIP_HORIZONTAL);
        }
        if (input->KeyPressed(SDL_SCANCODE_D))
        {
            sprite->Flip(SDL_FLIP_NONE);
        }

        displace.Normalise();
        displace *= (timer->DeltaTime() * 150.f);

        transform->Translate(displace);
	}

};