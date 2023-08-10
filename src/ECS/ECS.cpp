#include "src/ECS/ECS.hpp"

void Entity::addGroup(Group mGroup)
{
	groupBitSet[mGroup] = true;
	manager.AddToGroup(this, mGroup);
}