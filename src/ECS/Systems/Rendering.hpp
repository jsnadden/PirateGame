#pragma once
#include <entt.hpp>
#include "src/ECS/Components.hpp"
#include "src/Managers/Managers.hpp"

class Rendering
{
public:

	static void DrawMapTiles(entt::registry* reg)
	{
		for (auto [ent, trans, tile] : reg->view<TransformComponent, TileComponent>().each())
		{
            if (tile.visible)
            {
                SDL_Rect destRect{
                    static_cast<int>(trans.Position()->x - Camera::GetInstance()->OriginX()),
                    static_cast<int>(trans.Position()->y - Camera::GetInstance()->OriginY()),
                    static_cast<int>(tile.srcRect.w * trans.Scale()->x),
                    static_cast<int>(tile.srcRect.h * trans.Scale()->y)
                };

                Graphics::GetInstance()->DrawTexture(tile.texture, &tile.srcRect, &destRect);
            }
		}
	}

    static void DrawSprites(entt::registry* reg)
    {
        for (auto [ent, trans, sprite] : reg->view<TransformComponent, SpriteComponent>().each())
        {
            SDL_Rect destRect{
                    static_cast<int>(trans.Position()->x - Camera::GetInstance()->OriginX()),
                    static_cast<int>(trans.Position()->y - Camera::GetInstance()->OriginY()),
                    static_cast<int>(sprite.srcRect.w * trans.Scale()->x),
                    static_cast<int>(sprite.srcRect.h * trans.Scale()->y)
            };

            destRect.x -= destRect.w / 2;
            destRect.y -= destRect.h / 2;

            Graphics::GetInstance()->DrawTexture(sprite.texture, &sprite.srcRect, &destRect);
        }
    }

	static void AnimateSprites(entt::registry* reg, float dt)
	{
		auto spriteView = reg->view<TransformComponent, SpriteComponent>();

		for (auto [entity, trans, sprite] : spriteView.each())
		{
			if (!sprite.animationComplete)
			{
				sprite.elapsedTime += dt;

				if (sprite.elapsedTime > sprite.endTime)
				{
					if (sprite.mode == SpriteComponent::loop)
					{
						sprite.elapsedTime = 0.0f;
					}
					else if (sprite.mode == SpriteComponent::once)
					{
						sprite.animationComplete = true;
						sprite.elapsedTime = sprite.endTime - sprite.timePerFrame;
					}
				}

				int frame = static_cast<int>(sprite.elapsedTime / sprite.timePerFrame);
				if (frame >= sprite.totalFrames)
				{
					frame = sprite.totalFrames - 1;
				}

				sprite.srcRect.x = sprite.srcRect.w * frame;
			}
		}
	}

	static void DrawColliders(entt::registry* reg)
	{
		for (auto [ent, trans, coll] : reg->view<TransformComponent, ColliderComponent>().each())
		{
			SDL_Rect destRect{
				trans.Position()->x + coll.collider.x * trans.Scale()->x - Camera::GetInstance()->OriginX(),
				trans.Position()->y + coll.collider.y * trans.Scale()->y - Camera::GetInstance()->OriginY(),
				coll.collider.w * trans.Scale()->x,
				coll.collider.h * trans.Scale()->y
			};

			Graphics::GetInstance()->DrawTexture(coll.colliderTexture, NULL, &destRect);
		}
	}


};