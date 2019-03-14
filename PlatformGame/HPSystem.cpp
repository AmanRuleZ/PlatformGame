#include "HPSystem.h"

void HPSystem::HPDown(HPComponent & HP, int amount)
{
	HP.armor -= amount;
	if (HP.armor < 0) {
		HP.hp += HP.armor;
		HP.armor = 0;
	}
	if (HP.hp < 0) HP.hp = 0;
}

void HPSystem::HPUp(HPComponent & HP)
{
	HP.hp = HP.maxHp;
}

void HPSystem::ArmUp(HPComponent & HP)
{
	HP.armor = HP.maxArmor;
}
