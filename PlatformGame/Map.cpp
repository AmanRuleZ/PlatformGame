#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map()
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char c;
	char tile;
	std::fstream mapFile;
	std::fstream colFile;
	std::fstream hpFile;
	std::fstream pickFile;
	std::fstream trapFile;
	std::fstream enemyFile;

	mapFile.open(path+".map");
	hpFile.open(path + ".hp");
	pickFile.open(path + ".pck");
	trapFile.open(path + ".trap");
	enemyFile.open(path + ".enm");

	int srcX, srcY;


	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			Entity& tile(manager.addEntity());
			int xpos = x * scaledSize; //pozycja X na mapie
			int ypos = y * scaledSize; //pozycja Y na mapie
			mapFile.get(c);
			srcY = atoi(&c) * tileSize; //wiersz tekstury
			mapFile.get(c);
			srcX = atoi(&c) * tileSize; //kolumna tekstury
			int debug = 0;
			if (srcX + srcY != 0) {
				tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
				tile.addGroup(Game::groupMap);
				tile.addComponent<ColliderComponent>("terrain", x*scaledSize, y*scaledSize, scaledSize);
				tile.addGroup(Game::groupColliders);
			}
			else {
				tile.destroy();
			}
			mapFile.ignore();

			hpFile.get(c);
			if (c != '0')
			{
				tile.addComponent<HPComponent>(atoi(&c) * 100, atoi(&c) * 100,0,0);
			}

			hpFile.ignore();

			Entity& pickup(manager.addEntity());
			pickFile.get(c);

			if (c == '1') {

				pickup.addComponent<TransformComponent>(xpos + 15, ypos+10, 32, 32, 1);
				pickup.addComponent<SpriteComponent>("heart", false, 0);
				pickup.addComponent<PickupComponent>(1);
				pickup.addComponent<ColliderComponent>("pickup", x*scaledSize + 5, y*scaledSize, scaledSize);
				pickup.addGroup(Game::groupPickups);
				pickup.addGroup(Game::groupPck);
			}
			else if (c == '2') {
				pickup.addComponent<TransformComponent>(xpos + 15, ypos + 10, 32, 32, 1);
				pickup.addComponent<SpriteComponent>("shield", false, 0);
				pickup.addComponent<PickupComponent>(2);
				pickup.addComponent<ColliderComponent>("pickup", x*scaledSize + 5, y*scaledSize, scaledSize);
				pickup.addGroup(Game::groupPickups);
				pickup.addGroup(Game::groupPck);
			}
			else if (c == '3') {
				pickup.addComponent<TransformComponent>(xpos + 15, ypos + 10, 32, 32, 1);
				pickup.addComponent<SpriteComponent>("rock2", false, 0);
				pickup.addComponent<PickupComponent>(3);
				pickup.addComponent<ColliderComponent>("pickup", x*scaledSize + 5, y*scaledSize, scaledSize);
				pickup.addGroup(Game::groupPickups);
				pickup.addGroup(Game::groupPck);
			}
			else if (c == '4') {
				pickup.addComponent<TransformComponent>(xpos+5,ypos,32,32,1.5);
				pickup.addComponent<SpriteComponent>("pistol2", false,-30);
				pickup.addComponent<PickupComponent>(4);
				pickup.addComponent<ColliderComponent>("pickup", x*scaledSize + 5, y*scaledSize, scaledSize);
				pickup.addGroup(Game::groupPickups);
				pickup.addGroup(Game::groupPck);
			}
			else if (c == '5') {
				pickup.addComponent<TransformComponent>(xpos+5, ypos, 32, 32, 1.5);
				pickup.addComponent<SpriteComponent>("AK2", false,-30);
				pickup.addComponent<PickupComponent>(5);
				pickup.addComponent<ColliderComponent>("pickup", x*scaledSize+5, y*scaledSize, scaledSize);
				pickup.addGroup(Game::groupPickups);
				pickup.addGroup(Game::groupPck);
			}
			else {
				pickup.destroy();
			}
			pickFile.ignore();

		

			Entity& trap(manager.addEntity());
			trapFile.get(c);

			switch (c) {
				case '1':
					trap.addComponent<TransformComponent>(xpos, ypos + 56, 32, 32, 1.5);
					trap.addComponent<SpriteComponent>("spikes", false, 0);
					trap.addComponent<TrapComponent>(1);
					trap.addComponent<ColliderComponent>("spikes", x*scaledSize, y*scaledSize, scaledSize);
					trap.addGroup(Game::groupTraps);
					break;

				case '2':

					trap.addComponent<TransformComponent>(xpos, ypos, 32, 32, 2);
					trap.addComponent<SpriteComponent>("shooter", false, 0);
					trap.addComponent<TrapComponent>(2);
					trap.addComponent<ColliderComponent>("shooter", x*scaledSize, y*scaledSize, scaledSize);
					trap.addGroup(Game::groupTraps);
					break;

				case '3':
					trap.addComponent<TransformComponent>(xpos, ypos, 32, 32, 2);
					trap.addComponent<SpriteComponent>("shooter", false, 90);
					trap.addComponent<TrapComponent>(3);
					trap.addComponent<ColliderComponent>("shooter", x*scaledSize, y*scaledSize, scaledSize);
					trap.addGroup(Game::groupTraps);
					break;

				case '4':
					trap.addComponent<TransformComponent>(xpos, ypos, 32, 32, 2);
					trap.addComponent<SpriteComponent>("shooter", false, 180);
					trap.addComponent<TrapComponent>(4);
					trap.addComponent<ColliderComponent>("shooter", x*scaledSize, y*scaledSize, scaledSize);
					trap.addGroup(Game::groupTraps);
					break;

				case '5':
					trap.addComponent<TransformComponent>(xpos, ypos, 32, 32, 2);
					trap.addComponent<SpriteComponent>("shooter", false, 270);
					trap.addComponent<TrapComponent>(5);
					trap.addComponent<ColliderComponent>("shooter", x*scaledSize, y*scaledSize, scaledSize);
					trap.addGroup(Game::groupTraps);
					break;

				default:
					trap.destroy();
					break;
			}
			trapFile.ignore();

			Entity& enemy(manager.addEntity());
			enemyFile.get(c);
			if (c != '0')
			{
				
				if (c == '9') {
					enemy.addComponent<TransformComponent>(xpos, ypos, 54, 32, 0.87);
					enemy.addComponent<SpriteComponent>("enemy", true, 0);
					enemy.addComponent<ColliderComponent>("enemy");
					enemy.addComponent<AIComponent>("enemy");
					enemy.addComponent<GunComponent>("AK", 64, 64, 1);
				}
				if (c == '8') {
					enemy.addComponent<TransformComponent>(xpos, ypos, 32, 64,0.9);
					enemy.addComponent<SpriteComponent>("dog", true, 0);
					enemy.addComponent<ColliderComponent>("dog");
					enemy.addComponent<AIComponent>("dog");
					enemy.addComponent<TrapComponent>(99);
					enemy.addGroup(Game::groupTraps);
				}

				enemyFile.get(c);
				int hp = atoi(&c);
				enemyFile.get(c);
				int armor = atoi(&c);
				enemy.addComponent<HPComponent>(hp*100, hp*100, armor*100, armor*100);

				enemy.addGroup(Game::groupCharacters);
				enemy.addGroup(Game::groupEnemies);
			}
			else {
				enemy.destroy();
			}
			enemyFile.ignore();
			

		}
	}

	mapFile.close();
	colFile.close();
	trapFile.close();
	pickFile.close();
	hpFile.close();
}