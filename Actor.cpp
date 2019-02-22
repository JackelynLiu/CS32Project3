// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth), current_world(sw), m_status(true)
{}

bool Actor::getStatus() const { return m_status; }

void Actor::setStatus(bool new_status) { m_status = new_status; }

MovingObjects::MovingObjects(StudentWorld* sw, int imageID, double x, double y)
	:Actor(sw, imageID, x, y, right, 0)
{}

bool MovingObjects::blocksMovement() { return true; }

Person::Person(StudentWorld* sw, int imageID, double x, double y)
	:MovingObjects(sw, imageID, x, y), m_infectedstatus(false), m_infectioncount(0)
{}

Penelope::Penelope(StudentWorld* sw, double x, double y)
	:Person(sw, IID_PLAYER, x, y)
{}

void Penelope::doSomething()
{
	if (getWorld()->getLives() == 0)
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
		double current_x = getX();
		double current_y = getY();
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (!(getWorld()->containsObstacle(current_x - 4, current_y)))
				moveTo(current_x - 4, current_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!(getWorld()->containsObstacle(current_x + 4, current_y)))
				moveTo(current_x + 4, current_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!(getWorld()->containsObstacle(current_x, current_y - 4)))
				moveTo(current_x, current_y - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!(getWorld()->containsObstacle(current_x, current_y + 4)))
				moveTo(current_x, current_y + 4);
			break;
		/*case KEY_PRESS_SPACE:
			break*/
		default:
			return;
		}
	}
}

Citizen::Citizen(StudentWorld* sw, double x, double y)
	:Person(sw, IID_CITIZEN, x, y)
{}

void Citizen::doSomething()
{
	if (!getStatus())
		return;

}

Zombie::Zombie(StudentWorld* sw, double x, double y)
	:Actor(sw, IID_ZOMBIE, x, y, right, 0)
{}

bool Zombie::blocksMovement() { return true; }

SmartZombie::SmartZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void SmartZombie::doSomething()
{}

DumbZombie::DumbZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void DumbZombie::doSomething()
{}

StillObjects::StillObjects(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:Actor(sw, imageID, x, y, dir, depth)
{}

bool StillObjects::blocksMovement() { return false; }

Wall::Wall(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_WALL,x, y, right, 0)
{}

void Wall::doSomething() {}

bool Wall::blocksMovement() { return true; }

Exit::Exit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_EXIT, x, y, right, 1)
{}

void Exit::doSomething()
{}

Pit::Pit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_PIT, x, y, right, 0)
{}

void Pit::doSomething() {}

Goodie::Goodie(StudentWorld* sw, int imageID, double x, double y)
	:StillObjects(sw, imageID, x, y, right, 1)
{}

VaccineGoodie::VaccineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_VACCINE_GOODIE, x, y)
{}

void VaccineGoodie::doSomething() {}

GasCanGoodie::GasCanGoodie(StudentWorld* sw, double x, double y)
	:Goodie(sw, IID_GAS_CAN_GOODIE, x, y)
{}

void GasCanGoodie::doSomething() {}

LandMineGoodie::LandMineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_LANDMINE_GOODIE, x, y)
{}

void LandMineGoodie::doSomething() {}

LandMine::LandMine(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_LANDMINE, x, y)
{}

void LandMine::doSomething() {}

Projectile::Projectile(StudentWorld* sw, int imageID, double x, double y, int dir)
	:StillObjects(sw, imageID, x, y, dir, 0)
{}

Flame::Flame(StudentWorld* sw, double x, double y, int dir)
	: Projectile(sw, IID_FLAME, x, y, dir)
{}

void Flame::doSomething() {}

Vomit::Vomit(StudentWorld* sw, double x, double y, int dir)
	: Projectile(sw, IID_VOMIT, x, y, dir)
{
	getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
}

void Vomit::doSomething() {}

