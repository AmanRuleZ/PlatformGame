#ifndef HPSystem_h
#define HPSystem_h

#include "HPComponent.h"
class HPSystem
{
public:
	static void HPDown(HPComponent & HP, int amount);
	static void HPUp(HPComponent & HP);
	static void ArmUp(HPComponent & HP);
};


#endif