#include "AssetManager.hpp"
#include "Components.hpp"

AssetManager::AssetManager(Manager* man)
{
	AssetManager::manager = man;
}

AssetManager::~AssetManager()
{}

void AssetManager::CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string id)
{
	int centerX = position.x + 16;
	int centerY = position.y + 16;

	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(centerX, centerY, 32, 32, 1);
	projectile.addComponent<SpriteComponent>("projectile", false);
	projectile.addComponent<ProjectileComponent>(range, speed, velocity);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::projectileGroup);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}