#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject
{
public:
	Actor(int imageID, double x, double y, int dir, int depth);
	//virtual ~Actor();
	virtual void doSomething() = 0;

private:
	//bool m_status;
};

class Penelope : public Actor
{
public:
	Penelope(double x, double y);
	virtual ~Penelope();
	virtual void doSomething();

private:
};

class Wall :public Actor
{
public:
	Wall(double x, double y);
	virtual ~Wall();
	virtual void doSomething();
};

//Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
