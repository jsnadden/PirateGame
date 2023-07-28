#pragma once
#include <string>
#include <fstream>
#include "Vector2D.hpp"
#include "Components.hpp"
#include "Managers.hpp"


class Map
{
public:

	Map(std::string path, int ts, int ms, ECSManager* man);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY, Group colliderGrp, Group tileGrp);
	void AddTile(int srcX, int srcY, int xPos, int yPos, Group tileGrp);

	int mapWidth;
	int mapHeight;
	
private:

	Camera* camera;

	ECSManager* manager;

	std::string texturePath;
	int tileSize;
	int mapScale;
	int scaledSize;
};

