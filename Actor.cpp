// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth), m_status(true)
{}

bool Actor::getStatus() const { return m_status; }

void Actor::setStatus(bool new_status) { m_status = new_status; }

Person::Person(int imageID, double x, double y)
	:Actor(imageID, x, y, right, 0), m_infectedstatus(false), m_infectioncount(0)
{}

bool Person::blocksMovement() { return true; }

Penelope::Penelope(double x, double y, StudentWorld* sw)
	:Person(IID_PLAYER, x, y), studw(sw)
{}

void Penelope::doSomething()
{
	if (studw->getLives() == 0)
		return;
	int ch;
	if (studw->getKey(ch))
	{
		double current_x = getX();
		double current_y = getY();
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (!(studw->containsObstacle(current_x - 4, current_y)))
				moveTo(current_x - 4, current_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!(studw->containsObstacle(current_x + 4, current_y)))
				moveTo(current_x + 4, current_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!(studw->containsObstacle(current_x, current_y - 4)))
				moveTo(current_x, current_y - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!(studw->containsObstacle(current_x, current_y + 4)))
				moveTo(current_x, current_y + 4);
			break;
		/*case KEY_PRESS_SPACE:
			break*/
		default:
			return;
		}
	}
}

Citizen::Citizen(double x, double y)
	:Person(IID_CITIZEN, x, y)
{}

void Citizen::doSomething()
{
	if (!getStatus())
		return;

}

Zombie::Zombie(double x, double y)
	:Actor(IID_ZOMBIE, x, y, right, 0)
{}

bool Zombie::blocksMovement() { return true; }

SmartZombie::SmartZombie(double x, double y)
	:Zombie(x, y)
{}

void SmartZombie::doSomething()
{}

DumbZombie::DumbZombie(double x, double y)
	:Zombie(x, y)
{}

void DumbZombie::doSomething()
{}

Wall::Wall(double x, double y)
	:Actor(IID_WALL,x, y, right, 0)
{}

void Wall::doSomething() {}

bool Wall::blocksMovement() { return true; }

Exit::Exit(double x, double y)
	:Actor(IID_EXIT, x, y, right, 1)
{}

void Exit::doSomething() {}
bool Exit::blocksMovement() { return false; }
