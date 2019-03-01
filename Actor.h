#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;
class Penelope;

class Actor :public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, double x, double y, int dir, int depth);
	//virtual ~Actor();
	virtual void doSomething() = 0;
	virtual bool blocksMovement() const = 0;
	virtual bool canbeDamaged() const;
	virtual bool canbeInfected() const;		//PENELOPE, CITIZEN
	virtual bool blocksFlame() const;		//WALL, EXIT
	virtual bool triggersCitizens() const;
	virtual bool threatensCitizens() const;

	
	bool getStatus() const;
	virtual void changeStatus();
	int gettickcount() const;
	void increasetickcount();

	virtual void pickup(Penelope* p);

	//try to replace
	virtual bool canbePickedUp() const;
	
	bool isAt(double x, double y);

	//make more general function
	virtual bool canactivateLandmine() const;
	virtual void useExitIfAppropriate();


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
	virtual bool canactivateLandmine() const;
};

class Person :public MovingObjects
{
public:
	Person(StudentWorld* sw, int imageID, double x, double y);
	virtual void doSomething() = 0;
	virtual bool canbeInfected() const;
	int getInfectionCount() const;
	void setInfectionCount(int n);

	bool getInfectedStatus() const;
	void setInfectedStatus(bool infected);

private:
	int m_infectioncount;
	bool m_infectedstatus;
};

class Penelope : public Person
{
public:
	Penelope(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	//virtual void useExitIfAppropriate();
	virtual bool triggersCitizens() const;
	/*virtual void dieByFallOrBurnIfAppropriate();
	virtual void pickUpGoodieIfAppropriate(Goodie* g);*/

	void increaseVaccines();
	void increaseFlameCharges();
	void increaseLandmines();
	int getNumVaccines() const;
	int getNumFlameCharges() const;
	int getNumLandmines() const;

private:
	int num_vaccines, num_landmines, num_flamecharges;

};


class Citizen :public Person
{
public:
	Citizen(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void changeStatus();
	virtual void useExitIfAppropriate();

};

class Zombie :public MovingObjects
{
public:
	Zombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void changeStatus();
	virtual bool triggersCitizens() const;
	virtual bool threatensCitizens() const;

protected:
	void setRandomDirection();
	void moveinDirection();

private:
	int movement_plan;
};

class SmartZombie :public Zombie
{
public:
	SmartZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void changeStatus();
};

class DumbZombie : public Zombie
{
public:
	DumbZombie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void changeStatus();

private:
	bool has_vaccine;
};

class Wall :public Actor
{
public:
	Wall(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual bool blocksMovement() const;
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
	virtual bool blocksFlame() const;
	//virtual void activateifAppropriate(Actor* a);
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
	virtual void doSomething();
	virtual bool canbeDamaged() const;
	virtual void pickup(Penelope* p) = 0;
	virtual bool canbePickedUp() const;
};

class VaccineGoodie :public Goodie
{
public:
	VaccineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickup(Penelope* p);
};

class GasCanGoodie :public Goodie
{
public:
	GasCanGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickup(Penelope* p);
};

class LandmineGoodie :public Goodie
{
public:
	LandmineGoodie(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	virtual void pickup(Penelope* p);
};

class Landmine :public StillObjects
{
public:
	Landmine(StudentWorld* sw, double x, double y);
	virtual void doSomething();
	void explode();

private:
	bool m_active;
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
	virtual bool canactivateLandmine() const;
};

class Vomit :public Projectile
{
public:
	Vomit(StudentWorld* sw, double x, double y, int dir);
	virtual void doSomething();
};

#endif // ACTOR_H_
