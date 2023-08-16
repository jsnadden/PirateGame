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

    for (int i = -1; i <=1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            loadedChunks[i][j] = new MapChunk("assets/tiles/terrain.png", tSize, scale, enttReg, i, j);
            loadedChunks[i][j]->LoadChunk("assets/maps/map0.txt", chunkSize, chunkSize);
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
