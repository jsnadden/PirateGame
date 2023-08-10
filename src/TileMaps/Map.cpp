#include "src/TileMaps/Map.hpp"

Map::Map(std::string path, int ts, int ms, ECSManager* man)
{
	texturePath = path;
	tileSize = ts;
	mapScale = ms;
	scaledSize = ts * ms;

	camera = Camera::GetInstance();

	manager = man;
}

Map::~Map()
{}

void Map::LoadMap(std::string path, int sizeX, int sizeY, Group colliderGrp, Group tileGrp)
{
	mapWidth = scaledSize * sizeX;
	mapHeight = scaledSize * sizeY;
	camera->SetMapSize(mapWidth, mapHeight);

	char c;
	std::fstream mapfile;
	mapfile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapfile.get(c);
			srcY = atoi(&c) * tileSize;
			mapfile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x * scaledSize, y * scaledSize, tileGrp);
			mapfile.ignore();
		}
	}

	mapfile.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			mapfile.get(c);

			if (c == '1')
			{
				auto& tileCollider(manager->addEntity());
				tileCollider.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				//tileCollider.getComponent<ColliderComponent>().SetRelative(0.25f, 0.25f, 0.5f, 0.5f);
				// TODO make this reference a group specific to the current state stateStack.top()
				tileCollider.addGroup(colliderGrp);
			}

			mapfile.ignore();
		}
	}

	mapfile.close();
}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos, Group tileGrp)
{
	auto& tile(manager->addEntity());
	tile.addComponent<TileComponent>(texturePath, srcX, srcY, tileSize, Vector2D(xPos, yPos), mapScale);
	tile.addGroup(tileGrp);
}