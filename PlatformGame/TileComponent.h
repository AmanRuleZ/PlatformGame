#ifndef TileComponent_h
#define TileComponent_h
#include "ECS.h"
#include <SDL.h> 


class TileComponent : public Component
{
public:	
	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;
	float angle = 0;
	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, float tscale, std::string id)
	{
		texture = Game::assets.GetTexture(id);

		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = tsize;
		srcRect.h = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = tsize * tscale;
		destRect.h = tsize * tscale;

	}

	void update() override
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, angle, SDL_FLIP_NONE);
	}

};

#endif