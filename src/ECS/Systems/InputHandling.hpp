#pragma once
#include <SDL.h>
#include <entt.hpp>
#include "src/ECS/Components.hpp"
#include "src/Managers/Managers.hpp"
#include "src/Structs/Vector2D.hpp"

class InputHandling
{
public:

    static void ControlMovement(entt::registry* reg)
    {
        auto input = Input::GetInstance();
        Vector2D velocity;

        if (input->KeyDown(SDL_SCANCODE_W))
        {
            velocity += VEC_UP;
        }
        if (input->KeyDown(SDL_SCANCODE_A))
        {
            velocity += VEC_LEFT;
        }
        if (input->KeyDown(SDL_SCANCODE_S))
        {
            velocity += VEC_DOWN;
        }
        if (input->KeyDown(SDL_SCANCODE_D))
        {
            velocity += VEC_RIGHT;
        }

        velocity.Normalise();
        velocity *= 150.f;

        for (auto [ent, vel, sprite, ctrl] : reg->view<VelocityComponent, SpriteComponent, ControlComponent>().each())
        {
            vel.SetVelocity(velocity);

            if (velocity.x < 0) sprite.Flip(SDL_FLIP_HORIZONTAL);
            if (velocity.x > 0) sprite.Flip(SDL_FLIP_NONE);

            if (velocity.IsZero()) sprite.Play("Idle");
            else sprite.Play("Move");
        }
    }

};