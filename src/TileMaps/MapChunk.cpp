#include "MapChunk.hpp"

MapChunk::MapChunk(std::string path, int size, int scale, entt::registry* reg, int offsetX, int offsetY)
{
	tileSheetPath = path;
	tileSize = size;
	mapScale = scale;
	scaledSize = size * scale;

	enttReg = reg;

	loaded = false;

	chunkOffsetX = offsetX;
	chunkOffsetY = offsetY;
}

MapChunk::~MapChunk()
{
	if (loaded) UnloadChunk();
	tileGrid.clear();
	enttReg = nullptr;
}

bool MapChunk::LoadChunk(std::string path, int sizeX, int sizeY)
{
	tilesWide = sizeX;
	tilesHigh = sizeY;

	char c;
	std::fstream mapfile;
	mapfile.open(path);
	bool success = !mapfile.fail();

	if (success)
	{
		int srcX, srcY;

		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				mapfile.get(c);
				srcY = atoi(&c) * tileSize;
				mapfile.get(c);
				srcX = atoi(&c) * tileSize;

				tileGrid[x][y] = enttReg->create();
				AddTile(tileGrid[x][y], srcX, srcY, x * scaledSize, y * scaledSize);

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
					enttReg->emplace<ColliderComponent>(tileGrid[x][y], Polygon(0,0,tileSize, tileSize));
				}

				mapfile.ignore();
			}
		}

		mapfile.close();

		loaded = true;
	}

	return success;
}

void MapChunk::UnloadChunk()
{
	for (int y = 0; y < tilesHigh; y++)
	{
		for (int x = 0; x < tilesWide; x++)
		{
			enttReg->destroy(tileGrid[x][y]);
			tileGrid[x][y] = entt::null;
		}
	}

	loaded = false;
}

void MapChunk::AddTile(entt::entity ent, int srcX, int srcY, int xPos, int yPos)
{
	enttReg->emplace<TagComponent>(ent, "map_tile");
	enttReg->emplace<TransformComponent>(ent, Vector2D(xPos + scaledSize * chunkOffsetX * tilesWide, yPos + scaledSize * chunkOffsetY * tilesHigh), Vector2D(mapScale, mapScale));
	enttReg->emplace<TileComponent>(ent, tileSheetPath, srcX, srcY, tileSize);
}