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

private:

	std::map<int, std::map<int, MapChunk*>> loadedChunks;

public:

	Map(entt::registry* reg, std::string path, int mapW, int mapH, int cSize, int tSize, int scale, int startX, int startY);
	~Map();

	void ShuffleWest();
	void ShuffleEast();
	void ShuffleNorth();
	void ShuffleSouth();
};

