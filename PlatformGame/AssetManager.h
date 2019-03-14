#ifndef AssetManager_h
#define AssetManager_h

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();


	void CreateProjectile(Vector2D pos, Vector2D vel, std::string id, int h, int w, float size, bool thrown, bool self);


	void AddTexture(std::string id, const char* path);

	void ChangeTexture(std::string id, const char* path);

	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

	void AddSFX(std::string id, std::string path);

	//Mix_Music* GetMusic(std::string id);
	std::string GetSFX(std::string id);

	void playSFX(std::string id, int loops, int channel);


private:
	Manager * manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, SDL_AudioSpec*> sounds;
	std::map<std::string, Mix_Music*> mMusic;
	std::map<std::string, std::string > mSFX;
};

#endif