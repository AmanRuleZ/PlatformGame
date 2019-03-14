#ifndef Projectile_h
#define Projectile_h

#include "ECS.h"
#include "Components.h"

class ProjectileComponent : public Component
{
public:
	TransformComponent * transform;
	SpriteComponent * sprite;
	ProjectileComponent(int rng, int sp) : range(rng), speed(sp)
	{}
	~ProjectileComponent();

	void init() override
	{
		transform ->&entity->getComponent<TransformComponent>();
	}

	void update() override
	{
	}

private:
	int range;
	int speed;
	int distance;

};

#endif