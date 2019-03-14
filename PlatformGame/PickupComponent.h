#ifndef PickupComponent_h
#define PickupComponent_h

#include "Components.h"


class PickupComponent : public Component
{
public:
	int id; // 1-hp, 2-armor, 3-kamienie, 4-pistolet, 5-ak, 6-portal
	PickupComponent(int ident) : id(ident){}
};


#endif