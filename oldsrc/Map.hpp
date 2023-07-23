#pragma once
#include <string>

class Map
{
public:

	Map(std::string id, int ts, int ms);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xPos, int yPos);
	
private:

	std::string textureID;
	int tileSize;
	int mapScale;
	int scaledSize;

};

