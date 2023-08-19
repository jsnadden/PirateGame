#pragma once
#include <string>
#include <fstream>
#include "src/ECS/Entity.hpp"
#include "src/ECS/Components.hpp"
#include "src/Structs/Vector2D.hpp"
#include "src/Managers/Managers.hpp"

class MapChunk
{
public:

	MapChunk(std::string path, int ts, int ms, entt::registry* reg, int offsetX = 0, int offsetY = 0);
	~MapChunk();

	bool LoadChunk(std::string path, int sizeX, int sizeY);
	void UnloadChunk();

	void AddTile(entt::entity ent, int srcX, int srcY, int xPos, int yPos);
	Polygon CreateCollider(bool& coll, int type);

	int tilesWide;
	int tilesHigh;
	int chunkOffsetX;
	int chunkOffsetY;

	std::map<int, std::map<int, entt::entity>> tileGrid;

	bool loaded;

private:

	entt::registry* enttReg;

	std::string tileSheetPath;

	int tileSize;
	int mapScale;
	int scaledSize;
};

