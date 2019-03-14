#ifndef SpriteComponent_h
#define SpriteComponent_h

#include "Components.h"
#include <SDL.h>
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"
#include "Vector2D.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	int frames = 0;
	int speed = 100;

public:
	bool animated = false;
	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	float angle = 0;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	/*SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 2, 200);
		Animation walk = Animation(1, 6, 100);
		Animation jump = Animation(2, 1, 500);
		Animation fall = Animation(3, 1, 500);
		Animation land = Animation(4, 1, 500);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("Jump", jump);
		animations.emplace("Fall", fall);
		animations.emplace("Land", land);


		Play("Idle");
		setTex(id);
	}*/

	SpriteComponent(std::string id, bool isAnimated, float ang)
	{
		animated = isAnimated;
		Animation idle;
		Animation walk;
		Animation jump;
		Animation fall;
		Animation land;
		if (id == "dog") {
			idle = Animation(0, 2, 200);
			walk = Animation(1, 5, 100);
			jump = Animation(2, 1, 500);
			fall = Animation(3, 1, 500);
			land = Animation(4, 1, 500);
		}
		else if (id == "portal") {
			idle = Animation(0, 2, 200);
			walk = Animation(0, 2, 100);
			jump = Animation(0, 2, 500);
			fall = Animation(0, 2, 500);
			land = Animation(0, 2, 500);
		}
		else {
			idle = Animation(0, 2, 200);
			walk = Animation(1, 6, 100);
			jump = Animation(0, 1, 200);
			fall = Animation(0, 1, 200);
			land = Animation(0, 1, 200);
		}

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("Jump", jump);
		animations.emplace("Fall", fall);
		animations.emplace("Land", land);
		angle = ang;
		Play("Idle");
		setTex(id);
	}



	~SpriteComponent()
	{
	}

	void setTex(std::string id) {
		texture = Game::assets.GetTexture(id);	
	}


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;


	}

	void update() override
	{
		
		if (animated)
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		srcRect.y = animIndex * transform->height;
		destRect.x = static_cast<int>(transform->position.x - Game::camera.x);
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y);
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, angle, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

};

#endif