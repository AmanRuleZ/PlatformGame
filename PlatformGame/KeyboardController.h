#ifndef KeyboardController_h
#define KeyboardController_h

#include "Components.h"
#include "GunComponent.h"
#define JUMP -8
#define LEFT -2
#define RIGHT 2


class KeyboardController : public Component
{
public:
	TransformComponent * transform;
	SpriteComponent * sprite;
	GunComponent * gun;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		gun = &entity->getComponent<GunComponent>();
		transform->movable = true;
	}

	void update() override
	{
		SDL_PumpEvents();
		if (Game::ev.type == SDL_KEYDOWN)
		{
			switch (Game::ev.key.keysym.sym) {
			case SDLK_w:
				if (!transform->isJumping) {
					transform->isJumping = true;
					if (sprite->animated)transform->onGround = false;
					transform->position.y -= 8;
					transform->velocity.y = JUMP;
					Game::assets.playSFX("jump", 0, 1);
				}
				
				break;

			case SDLK_a:

				if (transform->canMoveLeft) {
					transform->xdir = LEFT;
					transform->canMoveRight = true;
				}

				break;


			case SDLK_d:

				if (transform->canMoveRight) {
					transform->xdir = RIGHT;
					transform->canMoveLeft = true;
				}

				break;

			case SDLK_1:
				if (gun->gunCount[0] > 0 && !gun->Rpressed) {
					gun->actualGun = 0;
					gun->reloadTime = -1;
				}
				break;

			case SDLK_2:
				if (gun->gunCount[1] > 0 && !gun->Rpressed) {
					gun->actualGun = 1;
					gun->reloadTime = -1;
				}
				break;

			case SDLK_3:
				if (gun->gunCount[2] > 0 && !gun->Rpressed) {
					gun->actualGun = 2;
					gun->reloadTime = -1;
				}
				break;

			case SDLK_r:
				if (!gun->Rpressed && gun->gunAmmo[gun->actualGun] != gun->gunMaxAmmo[gun->actualGun] && gun->ammo[gun->actualGun] > 0) {
					gun->reloadTime = Game::time + 60;
					gun->soundPlayed = false;
					gun->Rpressed = true;
				}
				break;

			case SDLK_t:
				cout << transform->position << endl; //do debugowania

			default:
				break;
			}


		}

		if (Game::ev.type == SDL_KEYUP)
		{
			switch (Game::ev.key.keysym.sym) {
			case SDLK_w:
				//transform->velocity.y = 0;
				
				break;

			case SDLK_a:


				transform->xdir = 0;

				//sprite->spriteFlip = SDL_FLIP_NONE;
				break;

			case SDLK_s:
				//transform->velocity.y = 0;

				break;

			case SDLK_d:
				transform->xdir = 0;

				break;

			default:
				break;
			}
		}


		if (sprite->animated) {
			if (transform->velocity.x >= 0) {
				sprite->spriteFlip = SDL_FLIP_NONE;
			}
			else {
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			}
			if (transform->onGround) {
				if (transform->velocity.x != 0) sprite->Play("Walk");
				else sprite->Play("Idle");

			}
			else {
				if (transform->velocity.y < -3) {
					sprite->Play("Jump");
				}
				else if (transform->velocity.y < 1) {
					sprite->Play("Fall");
				}
				else sprite->Play("Land");
			}

		}

	}
};

#endif