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
	virtual bool blocksMovement() = 0;

private:
	bool m_status;
};

class Person :public Actor
{
public:
	Person(int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual bool blocksMovement();

private:
	bool m_infectedstatus;
	int m_infectioncount;
};

class Citizen : public Person
{
	Citizen(double x, double y);
	virtual void doSomething();
	//virtual bool blocksMovement();
};

class Penelope : public Person
{
public:
	Penelope(double x, double y, StudentWorld* sw);
	virtual void doSomething();

private:
	StudentWorld* studw;
};

class Wall :public Actor
{
public:
	Wall(double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement();
};

class Exit :public Actor
{
public:
	Exit(double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement();
};

//Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
