#ifndef GunComponent_h
#define GunComponent_h

#include "Components.h"
#include "MouseController.h"

class GunComponent : public Component
{
private:
	TransformComponent* transform;
	MouseController* mouse;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

public:
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	float angle = 0;
	bool player = false;

	bool haveGun = false;
	int actualGun = 2;
	int gunCount[3]{ 25,1,1 };
	int gunMaxCount[3] = { 25,1,1 };
	int gunAmmo[3] = { 0,8,30 }; //aktualnie w magazynku
	int ammo[3] = { 0,24,90 }; //aktualnie we wszystkich magazynkach
	int gunMaxAmmo[3] = { 0,8,30 }; //maksymalnie w magazynku
	int gunMaxAmmo2[3] = { 0,24, 90 }; //maksymalnie we wszystkich magazynkach
	int gunBehavior[3] = { 1,2,2 }; //  (0 - nie można rzucać - bicie z pięści, 1 - można rzucać, 2 - można rzucać i strzelać)
									// 0 - pięść, 1 - pistolet, 2 - AK 

	int x, y;
	float XProj, YProj;
	int gameTime = 0;
	int enemyGameTime = 0;
	bool shoots = false;
	bool clicked;
	int reloadTime = -1;
	bool soundPlayed = true;
	bool ammoFilled = false;
	bool Rpressed = false;
	bool isShooting = false;


	GunComponent(){}

	GunComponent(std::string id, int w, int h, float scale) {
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = w;
		srcRect.h = h;
		destRect.w = w * scale;
		destRect.h = h * scale;
		setTex(id);
	}


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		if (entity->hasComponent<MouseController>()) {
			mouse = &entity->getComponent<MouseController>();
			player = true;
			cout << "Mam mysz" << endl;
		}
		else cout << "Nie mam myszy" << endl;
	}

	void update() override
	{
		destRect.x = static_cast<int>(transform->position.x - Game::camera.x) - 18;
		destRect.y = static_cast<int>(transform->position.y - Game::camera.y) - 14;

		if (!player) {
			if (transform->xdir > 0) spriteFlip = SDL_FLIP_NONE;
			else if (transform->xdir < 0) spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else {
			angle = mouse->kat;
			spriteFlip = mouse->spriteFlip;
		}


		int pom;

		if (reloadTime > 0) {
			if (gunAmmo[actualGun] >= 0 && ammo[actualGun] > 0 && gunMaxAmmo[actualGun] != gunAmmo[actualGun] && actualGun != 0 && !soundPlayed && !ammoFilled) {
				Game::assets.playSFX("reload", 0, 2);
			}
			if (Game::time == reloadTime) {
				pom = gunMaxAmmo[actualGun] - gunAmmo[actualGun];
				if (ammo[actualGun] >= pom)
				{
					ammo[actualGun] -= pom;
					gunAmmo[actualGun] = gunMaxAmmo[actualGun];
					ammoFilled = true;

				}
				else if (ammo[actualGun] > 0) {
					gunAmmo[actualGun] += ammo[actualGun];
					ammo[actualGun] = 0;
					ammoFilled = true;
				}

				reloadTime = -1;
				Rpressed = false;
			}
			soundPlayed = true;
		}
		if (entity->hasComponent<MouseController>()) {
			if (mouse->goShoot) shooting(mouse->XProj, mouse->YProj, true);
			if (mouse->goThrow) throwing(mouse->XProj, mouse->YProj, true, mouse->rel, mouse->x, mouse->y);
		}
	}




	void setTex(std::string id) {
		texture = Game::assets.GetTexture(id);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, angle, spriteFlip);
	}

	void pickup(int pickupID) {
		switch (pickupID) {
		case 3:
			gunCount[0] = 25;
			break;

		case 4:
			gunCount[1] = 1;
			ammo[1] = gunMaxAmmo2[1];
			break;

		case 5:
			gunCount[2] = 1;
			ammo[2] = gunMaxAmmo2[2];
			break;

		default:
			break;
		}
	}

	void throwing(float XProj, float YProj, bool self, Vector2D rel, int x, int y) {


			int size;
			float scale;
			if (gunCount[actualGun] > 0 && actualGun != 999) {
				std::string gun;
				switch (actualGun) {
				case 0:
					gun = "rock2";
					size = 32;
					scale = 0.5;
					break;

				case 1:
					gun = "pistol2";
					size = 32;
					scale = 1;
					break;

				case 2:
					gun = "AK2";
					size = 32;
					scale = 1;
					break;

				default:
					gun = "blank";
					size = 32;
					break;
				}

				Game::assets.playSFX("throw", 0, 3);
				Game::assets.CreateProjectile(Vector2D(transform->position.x + XProj / 15.0f + 16, transform->position.y - YProj / 15.0f + 10), Vector2D((float)(x - rel.x) / 100.0f, (float)(y - rel.y) / 100.0f), gun, size, size, scale, true, true);
				gunCount[actualGun]--;
				//ammo[actualGun] = 0;
				gunAmmo[actualGun] = 0;
				if (gunCount[actualGun] == 0) {
					actualGun = 999;
					for (int i = 0; i < 3; i++) {
						if (gunCount[i] > 0) {
							actualGun = i;
							break;
						}
					}
				}
			}
			Game::ev.type = SDL_MOUSEBUTTONUP;
			mouse->goThrow = false;
	}

	void shooting(float XProj, float YProj, bool self)
	{
		if (mouse->gameTime == Game::time && mouse->isShooting) {
			if (mouse->shoots && !Rpressed) {
				if (gunAmmo[actualGun] > 0 && actualGun != 999) {
					Game::assets.CreateProjectile(Vector2D(transform->position.x + XProj / 15.0f + 14, transform->position.y - YProj / 15.0f + 12), Vector2D(XProj, -YProj), "projectile", 32, 32, 0.2, false, true);
					gunAmmo[actualGun]--;
					ammoFilled = false;
					Game::assets.playSFX("shoot", 0, 0);
				}
			}
			if (actualGun != 2) {
				Game::ev.type = SDL_MOUSEBUTTONUP;
				mouse->shoots = false;
				mouse->clicked = false;
			}
			else {
				mouse->gameTime = (Game::time + 6) % 1000000000;
			}
		}
	}

	void enemyGunRotation(int x1, int y1, int x2, int y2) {
		float scale;
		int finalX = -(x1 - x2);
		if (-finalX > 0) {
			spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		else
		{
			spriteFlip = SDL_FLIP_NONE;
		}

		float ang = (float)((int)y2 - (int)y1) / (float)finalX;
		angle = atan(ang) * 180.0f / M_PI;

		float XProj, YProj;

		if (finalX > 0) {
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
			else if (XProj + YProj > 25) { //skalowanie wektora
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

		if (enemyGameTime == Game::time) {
			Game::assets.CreateProjectile(Vector2D(transform->position.x + XProj / 15.0f + 14, transform->position.y - YProj / 15.0f + 12), Vector2D(XProj, YProj), "projectile", 32, 32, 0.2, false, false);
			Game::assets.playSFX("shoot", 0, 0);
			enemyGameTime += rand() % 60+10;
		}
		else if (enemyGameTime > Game::time + 100 || enemyGameTime < Game::time - 100) enemyGameTime = Game::time;
	}

};




#endif