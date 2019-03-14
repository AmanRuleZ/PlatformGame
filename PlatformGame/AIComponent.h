#ifndef AIComponent_h
#define AIComponent_h

#include "Components.h"

class AIComponent : public Component
{
public:
	std::string id;
	TransformComponent * transform;
	SpriteComponent * sprite;
	ColliderComponent * col;
	Vector2D ray[3];
	SDL_Rect FOV;
	bool goToRock = false; //pod¹¿anie za kamieniem - widzi kamieñ
	int triggered = 0; //0  - nie widzi, 1 - widzi i atakuje, 2 - widzi i nie atakuje
	AIComponent(std::string i) : id(i){}

	void init() override
	{
		if (id == "dog") {
			FOV.h = 128;
			FOV.w = 500;
		}
		else if (id == "enemy")
		{
			FOV.h = 400;
			FOV.w = 410;
		}
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		col = &entity->getComponent<ColliderComponent>();
		transform->xdir = 1.5;
		sprite->Play("Walk");
		transform->movable = true;
	}

	void update() override
	{
		sprite->Play("Idle");
		if(id=="dog") FOV.y = transform->position.y -64;
		else if (id == "enemy") FOV.y = transform->position.y - 200;
		if (transform->xdir > 0) {
			FOV.x = transform->position.x;
			sprite->spriteFlip = SDL_FLIP_NONE;
			sprite->Play("Walk");
		}
		else if (transform->xdir < 0) {
			FOV.x = transform->position.x - FOV.w;
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			sprite->Play("Walk");
		}

		if (id == "dog" && triggered == 1 && Game::time % 20 == 0) {
			Game::assets.playSFX("dog", 0, rand() % 54 + 10);
		}
	}
	void swap(int x) {
		if (x == 0) {
			transform->xdir = 1.5;
			transform->position.x += 2;
			
		}
		else {
			transform->xdir = -1.5;
			transform->position.x -= 2;
			
		}
	}

};

#endif