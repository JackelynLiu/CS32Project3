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
	virtual bool blocksMovement() const = 0;
	virtual bool canbeDamaged() const;
	virtual bool canbeInfected() const;
	virtual bool blocksFlame() const;
	bool getStatus() const;
	void setStatus(bool new_status);
	int gettickcount() const;
	void increasetickcount();
	virtual std::string defineObjectType() const = 0;
	StudentWorld* getWorld() { return current_world; }

private:
	StudentWorld* current_world;
	bool m_status;
	int m_tickcount;
};

class MovingObjects :public Actor
{
public:
	MovingObjects(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual bool blocksMovement() const;
	virtual bool canbeDamaged() const;
};

class Person :public MovingObjects
{
public:
	Person(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual bool canbeInfected() const;
	bool getInfectedStatus() const;
	void setInfectedStatus(bool infected);
	int getInfectionCount() const;
	void setInfectionCount(int n);
	virtual std::string defineObjectType() const;

private:
	bool m_infectedstatus;
	int m_infectioncount;
};

class Penelope : public Person
{
public:
	Penelope(StudentWorld* sw, double x, double y);
	virtual void doSomething();
private:

};


class Citizen :public Person
{
public:
	Citizen(StudentWorld* sw, double x, double y);
	virtual void doSomething();

};

class Zombie :public MovingObjects
{
public:
	Zombie(StudentWorld* sw, double x, double y);
	virtual void doSomething() = 0;
	virtual std::string defineObjectType() const;
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

class Wall :public Actor
{
public:
	Wall(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
	virtual std::string defineObjectType() const;
	virtual bool blocksFlame() const;
};

class StillObjects :public Actor
{
public:
	StillObjects(StudentWorld* sw, int imageID, double x, double y, int dir, int depth);
	virtual void doSomething() = 0;
	virtual bool blocksMovement() const;
};

class Exit :public StillObjects
{
public:
	Exit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual std::string defineObjectType() const;
	virtual bool blocksFlame() const;
};

class Pit :public StillObjects
{
public:
	Pit(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual std::string defineObjectType() const;
};

class Goodie :public StillObjects
{
public:
	Goodie(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual std::string defineObjectType() const;
	virtual bool canbeDamaged() const;
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

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
};

class Landmine :public StillObjects
{
public:
	Landmine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual std::string defineObjectType() const;
	virtual bool canbeDamaged() const;
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
	virtual std::string defineObjectType() const;
};

class Vomit :public Projectile
{
public:
	Vomit(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
	virtual std::string defineObjectType() const;
};

#endif // ACTOR_H_
