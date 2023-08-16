#pragma once

#include "src/TileMaps/MapChunk.hpp"


class Map
{
public:

	std::string tileSheetPath;
	entt::registry* enttReg;
	int mapWidth;
	int mapHeight;
	int chunkSize;
	int tileSize;
	int mapScale;

private:

	std::map<int, std::map<int, MapChunk*>> loadedChunks;

public:

	Map(entt::registry* reg, std::string path, int mapW, int mapH, int cSize, int tSize, int scale, int startX, int startY);
	~Map();

	void Teleport(int chunkX, int chunkY);
	void ShuffleWest(int chunkX, int chunkY);
	void ShuffleEast(int chunkX, int chunkY);
	void ShuffleNorth(int chunkX, int chunkY);
	void ShuffleSouth(int chunkX, int chunkY);
};

