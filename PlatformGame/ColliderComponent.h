#ifndef ColliderComponent_h
#define ColliderComponent_h

#include <string>
#include <SDL.h>
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;
	SDL_Rect srcR,destR;
	float angle = 0;
	TransformComponent* transform;

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size) : tag(t)
	{
		collider.x = xpos;
		collider.y = ypos;
		collider.h = size;
		collider.w = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		srcR = { 0,0,32,32 };
		destR = { collider.x, collider.y, collider.w, collider.h };

		
	}

	void update() override
	{

		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.h = transform->height * transform->scale;
			collider.w = transform->width * transform->scale;
			if (tag == "projectile") {
				collider.h *= 1.5;
				collider.w *= 4.5;
			}
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;

	}

	void draw() override
	{
		SDL_Rect debug = collider;
		debug.x = destR.x;
		debug.y = destR.y;
		//SDL_RenderDrawRect(Game::renderer, &debug); //do sprawdzania colliderów
	}

};

#endif