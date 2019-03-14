#ifndef TrapComponent_h
#define TrapComponent_h

#include "Components.h"

class TrapComponent : public Component
{
public:
	int id;
	int toDown = 0;
	TransformComponent* transform;
	int time = 0;
	int delay=0;
	Vector2D position;
	TrapComponent(int x) : id(x){}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		position = transform->position;
		//delay = rand() % 60;
	}

	void update() override
	{
		if (transform->position.x < Game::camera.x + Game::camera.w &&
			transform->position.x > Game::camera.x &&
			transform->position.y < Game::camera.y + Game::camera.h &&
			transform->position.y > Game::camera.y) {
			if (time < Game::time && toDown > 0) {
				transform->position.y += 2;
				toDown -= 1;
			}

			if (Game::time % 60 == delay && id > 1 && id < 6) {
				shoot();
			}
		}
	}

	bool catched( TransformComponent & body, TransformComponent & head) {
		if (id == 1) {
			catched();
		}
		if (id < 2 || id > 5 && id != 98 && id !=100) {
			hited(body, head);
			return true;
		}
		return false;
		//return true;
	}

	void catched()
	{
		if (id == 1) {
			toDown = 20;
			transform->position.y = position.y - 40;
			Game::assets.playSFX("spikes", 0, rand()%54 + 10);
		}
	}

	void hited(TransformComponent & body, TransformComponent & head) {

		if (body.velocity.y < 0) {
			body.velocity.y = 5;
			head.velocity.y = 5;
		}
		else {
			body.velocity.y = -2;
			head.velocity.y = -2;
		}
		if (body.velocity.x == 0) {
			int chance = rand() % 2;
			if (chance == 0) {
				body.xdir = 4;
				head.xdir = 4;

			}
			else {
				body.xdir = -4;
				head.xdir = -4;
			}
		}
		else if (body.velocity.x < 0) {
			body.xdir = 4;
			head.xdir = 4;
		}
			else {
				body.xdir = -4;
				head.xdir = -4;
			}
	
		body.position.y -= 70;
		head.position.y -= 70;

		body.canMove = false;
		head.canMove = false;

		body.onGround = false;
		head.onGround = false;

		body.isJumping = true;
		head.isJumping = true;

	}

	void shoot() {
		int XProj, YProj,x=0,y=0;
		switch (id) {
		case 2:
			XProj = 0;
			YProj = 2;
			x = 32;
			break;

		case 3:
			XProj = -2;
			YProj = 0;
			y = 32;
			x = 30;
			break;

		case 4:
			XProj = 0;
			YProj = -2;
			x = 32;
			y = 55;
			break;

		case 5:
			XProj = 2;
			YProj = 0;
			y = 32;
			break;

		default:
			break;
		}
		Game::assets.CreateProjectile(Vector2D(transform->position.x +x, transform->position.y+y), Vector2D(XProj, YProj), "projectile", 32, 32, 0.2, false,false);
		Game::assets.playSFX("shooter", 0, 10);

	}

};

#endif