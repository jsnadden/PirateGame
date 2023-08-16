#include "src/TileMaps/Map.hpp"

//for (int i = 0; i < 3; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {
    //        mapChunks[i][j] = new MapChunk("assets/tiles/terrain.png", tileSize, mapScale, &enttReg, i-1, j-1);
    //        mapChunks[i][j]->LoadChunk("assets/maps/map0.txt", mapSize, mapSize);
    //    }
    //}

Map::Map(entt::registry* reg, std::string path, int mapW, int mapH, int cSize, int tSize, int scale, int startX, int startY)
{
    enttReg = reg;
    mapWidth = mapW;
    mapHeight = mapH;
    chunkSize = cSize;

    std::string mapPath;

    for (int i = -1; i <=1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            loadedChunks[i][j] = new MapChunk("assets/tiles/terrain.png", tSize, scale, enttReg, i, j);

            mapPath = "assets/maps/ocean_" + std::to_string(startX + i) + "_" + std::to_string(startY + j) + ".txt";
            loadedChunks[i][j]->LoadChunk(mapPath, chunkSize, chunkSize);
        }
    }
}

Map::~Map()
{
    enttReg = nullptr;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            delete loadedChunks[i][j];
        }
    }
}

void Map::ShuffleWest()
{
}

void Map::ShuffleEast()
{
}

void Map::ShuffleNorth()
{
}

void Map::ShuffleSouth()
{
}
