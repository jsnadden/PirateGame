#include "src/TileMaps/Map.hpp"

//for (int i = 0; i < 3; i++)
    //{
    //    for (int j = 0; j < 3; j++)
    //    {
    //        mapChunks[i][j] = new MapChunk("assets/tiles/terrain.png", tileSize, mapScale, &enttReg, i-1, j-1);
    //        mapChunks[i][j]->LoadChunk("assets/maps/map0.txt", mapSize, mapSize);
    //    }
    //}

Map::Map(entt::registry* reg, std::string name, std::string path, int mapW, int mapH, int cSize, int tSize, int scale, int startX, int startY)
{
    enttReg = reg;

    mapName = name;
    tileSheetPath = path;

    mapWidth = mapW;
    mapHeight = mapH;
    chunkSize = cSize;
    tileSize = tSize;
    mapScale = scale;

    std::string mapPath;

    for (int i = -1; i <=1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            loadedChunks[i][j] = new MapChunk(tileSheetPath, tSize, scale, enttReg, startX + i, startY + j);

            mapPath = "assets/maps/" + mapName + "_" + std::to_string(startX + i) + "_" + std::to_string(startY + j) + ".txt";
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

void Map::Teleport(int chunkX, int chunkY)
{
    std::string mapPath;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            delete loadedChunks[i][j];

            loadedChunks[i][j] = new MapChunk(tileSheetPath, tileSize, mapScale, enttReg, chunkX + i, chunkY + j);

            mapPath = "assets/maps/" + mapName + "_" + std::to_string(chunkX + i) + "_" + std::to_string(chunkY + j) + ".txt";
            loadedChunks[i][j]->LoadChunk(mapPath, chunkSize, chunkSize);
        }
    }
}

void Map::ShuffleWest(int chunkX, int chunkY)
{
    std::string mapPath;

    for (int j = -1; j <= 1; j++)
    {
        delete loadedChunks[1][j];
        loadedChunks[1][j] = loadedChunks[0][j];
        loadedChunks[0][j] = loadedChunks[-1][j];
        loadedChunks[-1][j] = new MapChunk(tileSheetPath, tileSize, mapScale, enttReg, chunkX - 1, chunkY + j);

        mapPath = "assets/maps/" + mapName + "_" + std::to_string(chunkX - 1) + "_" + std::to_string(chunkY + j) + ".txt";
        loadedChunks[-1][j]->LoadChunk(mapPath, chunkSize, chunkSize);
    }
}

void Map::ShuffleEast(int chunkX, int chunkY)
{
    std::string mapPath;

    for (int j = -1; j <= 1; j++)
    {
        delete loadedChunks[-1][j];
        loadedChunks[-1][j] = loadedChunks[0][j];
        loadedChunks[0][j] = loadedChunks[1][j];
        loadedChunks[1][j] = new MapChunk(tileSheetPath, tileSize, mapScale, enttReg, chunkX + 1, chunkY + j);

        mapPath = "assets/maps/" + mapName + "_" + std::to_string(chunkX + 1) + "_" + std::to_string(chunkY + j) + ".txt";
        loadedChunks[1][j]->LoadChunk(mapPath, chunkSize, chunkSize);
    }
}

void Map::ShuffleNorth(int chunkX, int chunkY)
{
    std::string mapPath;

    for (int i = -1; i <= 1; i++)
    {
        delete loadedChunks[i][1];
        loadedChunks[i][1] = loadedChunks[i][0];
        loadedChunks[i][0] = loadedChunks[i][-1];
        loadedChunks[i][-1] = new MapChunk(tileSheetPath, tileSize, mapScale, enttReg, chunkX + i, chunkY - 1);

        mapPath = "assets/maps/" + mapName + "_" + std::to_string(chunkX + i) + "_" + std::to_string(chunkY -1) + ".txt";
        loadedChunks[i][-1]->LoadChunk(mapPath, chunkSize, chunkSize);
    }
}

void Map::ShuffleSouth(int chunkX, int chunkY)
{
    std::string mapPath;

    for (int i = -1; i <= 1; i++)
    {
        delete loadedChunks[i][-1];
        loadedChunks[i][-1] = loadedChunks[i][0];
        loadedChunks[i][0] = loadedChunks[i][1];
        loadedChunks[i][1] = new MapChunk(tileSheetPath, tileSize, mapScale, enttReg, chunkX + i, chunkY + 1);

        mapPath = "assets/maps/" + mapName + "_" + std::to_string(chunkX + i) + "_" + std::to_string(chunkY + 1) + ".txt";
        loadedChunks[i][1]->LoadChunk(mapPath, chunkSize, chunkSize);
    }
}
