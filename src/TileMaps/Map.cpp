#include "src/TileMaps/Map.hpp"

Map::Map(std::string path, int ts, int ms, entt::registry* reg)
{
	tileSheetPath = path;
	tileSize = ts;
	mapScale = ms;
	scaledSize = ts * ms;

	camera = Camera::GetInstance();

	enttReg = reg;
}

Map::~Map()
{}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
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

			tileGrid[std::pair(x, y)] = enttReg->create();
			AddTile(tileGrid[std::pair(x, y)], srcX, srcY, x * scaledSize, y * scaledSize);

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
				enttReg->emplace<ColliderComponent>(tileGrid[std::pair(x, y)], 0, 0, tileSize, tileSize);
			}

			mapfile.ignore();
		}
	}

	mapfile.close();
}

void Map::AddTile(entt::entity ent, int srcX, int srcY, int xPos, int yPos)
{
	enttReg->emplace<TagComponent>(ent, "map_tile");
	enttReg->emplace<TransformComponent>(ent, Vector2D(xPos, yPos), Vector2D(mapScale, mapScale));
	enttReg->emplace<TileComponent>(ent, tileSheetPath, srcX, srcY, tileSize);
}