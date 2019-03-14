#include "AssetManager.h"
#include "Components.h"
#include "SDL_mixer.h"
AssetManager::AssetManager(Manager * man) : manager(man)
{

}

AssetManager::~AssetManager()
{
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, std::string id, int h, int w, float size, bool thrown,bool self)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, h, w, size);
	projectile.addComponent<SpriteComponent>(id, false, 0);
	projectile.addComponent<ProjectileComponent>(vel, thrown);
	projectile.addComponent<ColliderComponent>("projectile");
	if (thrown) projectile.addComponent<TrapComponent>(100);
	else if(self) projectile.addComponent<TrapComponent>(98);
	else projectile.addComponent<TrapComponent>(99);
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char * path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

void AssetManager::ChangeTexture(std::string id, const char * path)
{
	textures.erase(id);
	cout << "Texture erased" << endl;
	textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture * AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize) {
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}


void AssetManager::AddSFX(std::string id, std::string path)
{
	mSFX.emplace(id, path);
}



std::string AssetManager::GetSFX(std::string id)
{
	return mSFX[id];
}

void AssetManager::playSFX(std::string id, int loops, int channel)
{
	Mix_Chunk *chunk;
	chunk = Mix_LoadWAV(Game::assets.GetSFX(id).c_str());
	Mix_PlayChannel(channel, chunk, loops);
}
