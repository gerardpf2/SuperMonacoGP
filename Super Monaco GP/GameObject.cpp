#include "GameObject.h"

GameObject::GameObject(float z, unsigned int color) :
	z(z), color(color)
{ }

GameObject::~GameObject()
{ }

float GameObject::getZ() const
{
	return z;
}