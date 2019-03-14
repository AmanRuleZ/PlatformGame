#ifndef TileHPComponent_h
#define TileComponent_h
#include "ECS.h"
#include <SDL.h> 

class TileHPComponent : public Component
{
public:
	int HP;
	TileHPComponent(int hp) :HP(hp)
	{
	}

	~TileHPComponent() {}
};


#endif