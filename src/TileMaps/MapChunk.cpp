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
				srcY = atoi(&c);
				mapfile.get(c);
				srcX = atoi(&c);

				tileGrid[x][y] = enttReg->create();
				AddTile(tileGrid[x][y], srcX * tileSize, srcY * tileSize, x * scaledSize, y * scaledSize);

				int tileType = srcY * 10 + srcX;
				
				if (tileType > 8) tileType *= 10;

				bool collider = false;
				
				AABB box = CreateAABBCollider(collider, tileType);
				if (collider)
				{
					enttReg->emplace<AABBCollider>(tileGrid[x][y], box);

					if (tileType > 8)
					{
						box = CreateAABBCollider(collider, tileType + 1);
						if (collider) enttReg->get<AABBCollider>(tileGrid[x][y]).boundingBoxes.emplace_back(box);
					}
				}

				mapfile.ignore();
			}
		}

		/*mapfile.ignore();

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
		}*/

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

Polygon MapChunk::CreatePolyCollider(bool& coll, int type)
{
	Polygon p;

	// these choices are specific to the ordering of the current tilesheet
	switch (type)
	{
	case 1:
		coll = true;
		p = Polygon(tileSize / 2, 0, tileSize / 2, tileSize);
		break;
	case 2:
		coll = true;
		p = Polygon(tileSize / 2, 0, tileSize / 2, tileSize / 2);
		break;
	case 3:
		coll = true;
		p = Polygon(tileSize / 2, tileSize / 2, tileSize / 2, tileSize / 2);
		break;
	case 4:
		coll = true;
		p = Polygon(0, tileSize / 2, tileSize, tileSize / 2);
		break;
	case 5:
		coll = true;
		p = Polygon(0, tileSize / 2, tileSize / 2, tileSize / 2);
		break;
	case 6:
		coll = true;
		p = Polygon(0, 0, tileSize / 2, tileSize);
		break;
	case 7:
		coll = true;
		p = Polygon(0, 0, tileSize / 2, tileSize / 2);
		break;
	case 8:
		coll = true;
		p = Polygon(0, 0, tileSize, tileSize / 2);
		break;
	case 9:
		coll = true;
		p = Polygon({ Vector2D(0, tileSize), Vector2D(tileSize / 2, tileSize),
			Vector2D(tileSize, tileSize / 2), Vector2D(tileSize, 0) });
		break;
	case 10:
		coll = true;
		p = Polygon({ Vector2D(0, 0), Vector2D(0, tileSize / 2),
			Vector2D(tileSize / 2, tileSize), Vector2D(tileSize, tileSize) });
		break;
	case 11:
		coll = true;
		p = Polygon({ Vector2D(0, tileSize / 2), Vector2D(0, tileSize),
			Vector2D(tileSize, 0), Vector2D(tileSize / 2, 0) });
		break;
	case 12:
		coll = true;
		p = Polygon({ Vector2D(0, 0), Vector2D(tileSize, tileSize),
			Vector2D(tileSize, tileSize / 2), Vector2D(tileSize / 2, 0) });
		break;
	default:
		coll = false;
		break;
	}

	return p;
}

AABB MapChunk::CreateAABBCollider(bool& coll, int type)
{
	AABB b;

	// these choices are specific to the ordering of the current tilesheet
	switch (type)
	{
	case 1:
		coll = true;
		b = AABB(tileSize / 2, 0, tileSize / 2, tileSize);
		break;
	case 2:
		coll = true;
		b = AABB(tileSize / 2, 0, tileSize / 2, tileSize / 2);
		break;
	case 3:
		coll = true;
		b = AABB(tileSize / 2, tileSize / 2, tileSize / 2, tileSize / 2);
		break;
	case 4:
		coll = true;
		b = AABB(0, tileSize / 2, tileSize, tileSize / 2);
		break;
	case 5:
		coll = true;
		b = AABB(0, tileSize / 2, tileSize / 2, tileSize / 2);
		break;
	case 6:
		coll = true;
		b = AABB(0, 0, tileSize / 2, tileSize);
		break;
	case 7:
		coll = true;
		b = AABB(0, 0, tileSize / 2, tileSize / 2);
		break;
	case 8:
		coll = true;
		b = AABB(0, 0, tileSize, tileSize / 2);
		break;
	
	case 90:
		coll = true;
		b = AABB(0, 0, tileSize, tileSize / 2);
		break;
	case 100:
		coll = true;
		b = AABB(0, 0, tileSize, tileSize / 2);
		break;
	case 110:
		coll = true;
		b = AABB(0, tileSize / 2, tileSize, tileSize / 2);
		break;
	case 120:
		coll = true;
		b = AABB(0, tileSize / 2, tileSize, tileSize / 2);
		break;
	case 91:
		coll = true;
		b = AABB(0, 0, tileSize / 2, tileSize);
		break;
	case 101:
		coll = true;
		b = AABB(tileSize / 2, 0, tileSize / 2, tileSize);
		break;
	case 111:
		coll = true;
		b = AABB(tileSize / 2, 0, tileSize / 2, tileSize);
		break;
	case 121:
		coll = true;
		b = AABB(0, 0, tileSize / 2, tileSize);
		break;

	default:
		coll = false;
		break;
	}

	return b;
}
