#ifndef MouseController_h
#define MouseController_h

#include "GunComponent.h"
#include "Components.h"
#include "Game.h"

class MouseController : public Component
{
public:

	int x, y;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	float kat = 0.0;
	float scale;
	float ang;
	int finalX, finalY;
	Vector2D rel;
	float XProj, YProj;

	bool goShoot = false;
	bool goThrow = false;
	bool shoots = false;
	bool clicked = false;
	bool isShooting = false;
	int gameTime;
	MouseController(bool hd) : head(hd){}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		SDL_PumpEvents();
		SDL_GetMouseState(&x, &y);
		float scale;
		rel = transform->relPos;
		finalX = -(rel.x - x);
		if (rel.x - x > 0) {
			spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else
		{
			spriteFlip = SDL_FLIP_NONE;
		}
		if (!head) scale = 1;
		else if (rel.y - y > 0)
		{
			scale = 1.3;
		}
		else {
			scale = 0.7;
		}

		int projPos;
		ang = (float)((int)transform->relPos.y - (int)y) / (float)finalX;
		float angle = atan(ang) * 180.0f / M_PI;
		kat = -angle * scale;
		if (head) {
			sprite->angle = kat;
			sprite->spriteFlip = spriteFlip;
		}else{
			if (Game::ev.type == SDL_MOUSEBUTTONDOWN && !clicked)
			{
				gameTime = Game::time;
				clicked = true;
			}

			if (Game::ev.type != SDL_MOUSEBUTTONUP && clicked)
			{
				shoots = true;
				scale = 1;

				int projPos;
				if (spriteFlip == SDL_FLIP_NONE) projPos = 10;
				else projPos = -25;
				if (rel.x - x == 0 && rel.y - y < 0) ang = 99.9f;
				if (rel.x - x == 0 && rel.y - y > 0) ang = -99.9f;

				if (rel.x - x < 0) {
					XProj = 20;
					YProj = ang * XProj;
					if (YProj > 300) {
						XProj = 0;
						YProj = 25;
					}
					else if (YProj < -300) {
						XProj = 0;
						YProj = -25;
					}
					else if (XProj + YProj > 25) {
						while (XProj + YProj > 25) {
							if (XProj + YProj > 150) {
								XProj = XProj / 10.0f;
								YProj = YProj / 10.0f;
							}
							else if (XProj + YProj > 50) {
								XProj = XProj / 2.0f;
								YProj = YProj / 2.0f;
							}
							else {
								XProj--;
								YProj -= ang;
							}
						}
					}
					if (-XProj + YProj < -25) {

						while (-XProj + YProj < -25) {
							if (-XProj + YProj < -150) {
								XProj = XProj / 10.0f;
								YProj = YProj / 10.0f;
							}
							else if (-XProj + YProj < -50) {
								XProj = XProj / 2.0f;
								YProj = YProj / 2.0f;
							}
							else {
								XProj--;
								YProj -= ang;
							}

						}
					}
				}
				else {
					XProj = -20;
					YProj = ang * XProj;
					if (YProj > 300) {
						XProj = 0;
						YProj = 25;
					}
					else if (YProj < -300) {
						XProj = 0;
						YProj = -25;
					}
					else if (-XProj + YProj > 25) {
						while (-XProj + YProj > 25)
						{
							if (-XProj + YProj > 150) {
								XProj = XProj / 10.0f;
								YProj = YProj / 10.0f;
							}
							else if (-XProj + YProj > 50) {
								XProj = XProj / 2.0f;
								YProj = YProj / 2.0f;
							}
							else {
								XProj++;
								YProj += ang;
							}
						}
					}
					if (XProj + YProj < -25) {
						while (XProj + YProj < -25) {
							if (XProj + YProj < -150)
							{
								XProj = XProj / 10.0f;
								YProj = YProj / 10.0f;
							}
							else if (XProj + YProj < -50) {
								XProj = XProj / 2.0f;
								YProj = YProj / 2.0f;
							}
							else {
								XProj++;
								YProj += ang;
							}
						}
					}
				}

				if ((int)XProj == 0 && (int)YProj == 0) YProj = 20;


				if (Game::ev.button.button == SDL_BUTTON_RIGHT)
				{
					//gun->throwing(XProj, YProj, true, rel);
					goThrow = true;
				}

				if (Game::ev.button.button == SDL_BUTTON_LEFT && Game::ev.type == SDL_MOUSEBUTTONDOWN) {
					if (!isShooting) {
						isShooting = true;
						gameTime = Game::time;

					}
				}

				if (isShooting) {
					goShoot = true;
				}

			}

		}
		if (Game::ev.type == 1026) {
			gameTime = -1;
			shoots = false;
			clicked = false;
			isShooting = false;
		}
	}
	private:
		bool head;
		TransformComponent * transform;
		SpriteComponent * sprite;
};

#endif