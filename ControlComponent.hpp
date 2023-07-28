#pragma once
#include "Components.hpp"
#include "Managers.hpp"
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

	void EarlyUpdate() override
	{
        Vector2D vel;
        sprite->Play("Idle");

        if (input->KeyDown(SDL_SCANCODE_W))
        {
            vel += VEC_UP;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_A))
        {
            vel += VEC_LEFT;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_S))
        {
            vel += VEC_DOWN;
            sprite->Play("Sail");
        }
        if (input->KeyDown(SDL_SCANCODE_D))
        {
            vel += VEC_RIGHT;
            sprite->Play("Sail");
        }
        
        if (vel.x > 0)
        {
            sprite->Flip(SDL_FLIP_NONE);
        }
        if (vel.x < 0)
        {
            sprite->Flip(SDL_FLIP_HORIZONTAL);
        }

        vel.Normalise();

        // TODO this speed should be set by other systems!!
        vel *= 150.f;
        transform->SetVelocity(vel);
	}

};