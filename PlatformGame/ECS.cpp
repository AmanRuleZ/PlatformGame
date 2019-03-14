#include "ECS.h"

void Entity::addGroup(Group mGroup)
{
	if (!groupBitset[mGroup]) {
		groupBitset[mGroup] = true;
		manager.AddToGroup(this, mGroup);
	}
}

