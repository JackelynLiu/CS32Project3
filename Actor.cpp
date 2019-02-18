// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth) {}

Penelope::Penelope(double x, double y)
	:Actor(IID_PLAYER, x, y, 0, 0)
{

}

Penelope::~Penelope()
{
	delete this;
}

void Penelope::doSomething() {}


Wall::Wall(double x, double y)
	:Actor(IID_WALL,x, y, 0, 0)
{

}

Wall::~Wall() {}

void Wall::doSomething() {}

