#ifndef TransformComponent_h
#define TransformComponent_h

#include "Components.h"
#include "Vector2D.h"
#include "Game.h"

class TransformComponent : public Component
{
public: 

	Vector2D position;
	Vector2D relPos;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	float scale = 1;

	bool onGround = false;
	bool isJumping = false;

	bool canMoveLeft = true;
	bool canMoveRight = true;


	int speed = 3;

	bool anyCollision = false;
	bool canMove = true;

	int xdir = 0;

	bool movable = false;

	TransformComponent(float sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
		velocity.y = 0;
	}


	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float x, float y)
	{
		position.Zero();
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}

	void init() override {
		velocity.Zero();
	}

	void update() override
	{
		if (movable && canMove) {
			if (xdir > 0 && canMoveRight)
				velocity.x = xdir;
			else if (xdir < 0 && canMoveLeft)
				velocity.x = xdir;
			else velocity.x = 0;

			if (!canMove) velocity.x = xdir;

		}

		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

};


#endif