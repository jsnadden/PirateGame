#pragma once
#include <string>
#include <fstream>
#include "src/ECS/Entity.hpp"
#include "src/ECS/Components.hpp"
#include "src/Structs/Vector2D.hpp"
#include "src/Managers/Managers.hpp"


class Map
{
public:

	Map(std::string path, int ts, int ms, entt::registry* reg);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(entt::entity ent, int srcX, int srcY, int xPos, int yPos);

	int mapWidth;
	int mapHeight;

	std::map<std::pair<int, int>, entt::entity> tileGrid;
	
private:

	Camera* camera;

	entt::registry* enttReg;

	std::string tileSheetPath;

	int tileSize;
	int mapScale;
	int scaledSize;
};

