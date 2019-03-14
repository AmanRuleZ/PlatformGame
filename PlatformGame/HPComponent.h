#ifndef HPComponent_h
#define HPComponent_h

#include "Components.h"
#include "Game.h"

class HPComponent : public Component
{
public:
	int hp;
	int maxHp;
	int armor;
	int maxArmor;

	HPComponent(int h, int mh, int a, int ma) : hp(h), maxHp(mh), armor(a), maxArmor(ma){}
	void changeHP(int h, int mh, int a, int ma)
	{
		hp = h;
		maxHp = mh;
		armor = a;
		maxArmor = ma;
	}
};

#endif