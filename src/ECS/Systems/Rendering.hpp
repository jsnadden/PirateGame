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
                    static_cast<int>(trans.transform.position.x - Camera::GetInstance()->OriginX()),
                    static_cast<int>(trans.transform.position.y - Camera::GetInstance()->OriginY()),
                    static_cast<int>(tile.srcRect.w * trans.transform.scale.x),
                    static_cast<int>(tile.srcRect.h * trans.transform.scale.y)
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
                    static_cast<int>(trans.transform.position.x - Camera::GetInstance()->OriginX()),
                    static_cast<int>(trans.transform.position.y - Camera::GetInstance()->OriginY()),
                    static_cast<int>(sprite.srcRect.w * trans.transform.scale.x),
                    static_cast<int>(sprite.srcRect.h * trans.transform.scale.y)
            };

            destRect.x -= destRect.w / 2;
            destRect.y -= destRect.h / 2;

            Graphics::GetInstance()->DrawTexture(sprite.texture, &sprite.srcRect, &destRect, 0.0f, sprite.spriteFlip);
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
		for (auto [ent, trans, coll] : reg->view<TransformComponent, PolyCollider>().each())
		{
			int n = coll.polygon.vertices.size();

			for (int i = 0; i < n; i++)
			{
				int j = (i + 1) % n;
				Graphics::GetInstance()->DrawLine(coll.colour,
					trans.transform.position.x + coll.polygon.vertices[i].x * trans.transform.scale.x - Camera::GetInstance()->OriginX(),
					trans.transform.position.y + coll.polygon.vertices[i].y * trans.transform.scale.y - Camera::GetInstance()->OriginY(),
					trans.transform.position.x + coll.polygon.vertices[j].x * trans.transform.scale.x - Camera::GetInstance()->OriginX(),
					trans.transform.position.y + coll.polygon.vertices[j].y * trans.transform.scale.y - Camera::GetInstance()->OriginY());
			}
		}

		for (auto [ent, trans, coll] : reg->view<TransformComponent, AABBCollider>().each())
		{
			for (auto b : coll.boundingBoxes)
			{
				SDL_Rect bCast = {
					trans.transform.position.x + trans.transform.scale.x * b.x - Camera::GetInstance()->OriginX(),
					trans.transform.position.y + trans.transform.scale.y * b.y - Camera::GetInstance()->OriginY(),
					trans.transform.scale.x * b.w,
					trans.transform.scale.y * b.h };
				Graphics::GetInstance()->DrawRectangle(coll.colour, &bCast);
			}
		}
	}


};