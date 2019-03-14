#ifndef Map_h
#define Map_h
#include <string>

class Map
{
public:
	Map(std::string tID, int ms, int ts ); //mapFilePath, mapScale, tileSize
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
private: 
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
};

#endif