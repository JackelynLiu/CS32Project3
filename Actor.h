#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

class Actor :public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, double x, double y, int dir, int depth);
	//virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool blocksMovement() = 0;
	bool getStatus() const;
	void setStatus(bool new_status);
	StudentWorld* getWorld() { return current_world; }

private:
	StudentWorld* current_world;
	bool m_status;
};

class MovingObjects :public Actor
{
public:
	MovingObjects(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual bool blocksMovement();
};

class Person :public MovingObjects
{
public:
	Person(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;

private:
	bool m_infectedstatus;
	int m_infectioncount;
};

class Citizen : public Person
{
public:
	Citizen(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class Penelope : public Person
{
public:
	Penelope(StudentWorld* sw, double x, double y);
	virtual void doSomething();
private:

};

class Zombie :public Actor
{
public:
	Zombie(StudentWorld* sw, double x, double y);
	virtual void doSomething() = 0;
	virtual bool blocksMovement();
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class StillObjects :public Actor
{
public:
	StillObjects(StudentWorld* sw, int imageID, double x, double y, int dir, int depth);
	virtual void doSomething() = 0;
	virtual bool blocksMovement();
};

class Wall :public StillObjects
{
public:
	Wall(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement();
};

class Exit :public StillObjects
{
public:
	Exit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class Pit :public StillObjects
{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class Goodie :public StillObjects
{
public:
	Goodie(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class LandMineGoodie :public Goodie
{
public:
	LandMineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class LandMine :public Goodie
{
public:
	LandMine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class Projectile :public StillObjects
{
public:
	Projectile(StudentWorld* sw, int imageID, double x, double y, int dir);
	virtual void doSomething() = 0;
};

class Flame :public Projectile
{
public:
	Flame(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
};

class Vomit :public Projectile
{
public:
	Vomit(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
};
//Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
