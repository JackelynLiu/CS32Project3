#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int imageID, double x, double y, int dir, int depth);
	//virtual ~Actor();
	virtual void doSomething() = 0;

private:
	
};

class Penelope : public Actor
{
public:
	Penelope(double x, double y, StudentWorld* sw);
	virtual ~Penelope();
	virtual void doSomething();

private:
	StudentWorld* studw;
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
