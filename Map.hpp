#pragma once
#include <string>
#include "Vector2D.hpp"
#include "ECS.hpp"

class Map
{
public:

	Map(std::string path, int ts, int ms, ECSManager* man);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xPos, int yPos);

	int mapWidth;
	int mapHeight;
	
private:

	ECSManager* manager;

	std::string texturePath;
	int tileSize;
	int mapScale;
	int scaledSize;
};

