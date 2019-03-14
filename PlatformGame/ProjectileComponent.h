#ifndef Projectile_h
#define Projectile_h

#include "ECS.h"
#include "Components.h"
#include "Game.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Vector2D vel, bool thr) : velocity(vel), thrown(thr)
	{
	}

	~ProjectileComponent(){}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		
		// Generowanie tylko tych elementów, które s¹ w polu widzenia
		if (transform->position.x > Game::camera.x + Game::camera.w ||
			transform->position.x < Game::camera.x || 
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y)
		{
			entity->destroy();
		}
		if (thrown) {
			transform->velocity.y += 0.15;
			sprite->angle = angle;
			angle = angle + 15;
		}
	}


private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	int angle = 0;
	bool thrown;
	Vector2D velocity;
};


#endif