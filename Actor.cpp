// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth), current_world(sw), m_status(true), m_tickcount(0)
{}

bool Actor::getStatus() const { return m_status; }

void Actor::setStatus(bool new_status) { m_status = new_status; }

int Actor::gettickcount() const { return m_tickcount; }

void Actor::increasetickcount() { m_tickcount++; }

bool Actor::canbeDamaged() const { return false; }

bool Actor::canbeInfected() const { return false; }

MovingObjects::MovingObjects(StudentWorld* sw, int imageID, double x, double y)
	:Actor(sw, imageID, x, y, right, 0)
{}

bool MovingObjects::blocksMovement() const { return true; }

bool MovingObjects::canbeDamaged() const { return true; }

Person::Person(StudentWorld* sw, int imageID, double x, double y)
	:MovingObjects(sw, imageID, x, y), m_infectedstatus(false), m_infectioncount(0)
{}

void Person::doSomething()
{
	if (!getStatus())
		return;
	if (m_infectedstatus)
		m_infectioncount++;
}

std::string Person::defineObjectType() const { return "PERSON"; }

bool Person::canbeInfected() const { return true; }

bool Person::getInfectedStatus() const { return m_infectedstatus; }

void Person::setInfectedStatus(bool infected) { m_infectedstatus = infected; }

int Person::getInfectionCount() const { return m_infectioncount; }

void Person::setInfectionCount(int n) { m_infectioncount = n; }

Penelope::Penelope(StudentWorld* sw, double x, double y)
	:Person(sw, IID_PLAYER, x, y)
{}

void Penelope::doSomething()
{
	Person::doSomething();
	if (getInfectionCount() == 500)
	{
		setStatus(false);
		getWorld()->playSound(SOUND_PLAYER_DIE);
		return;
	}
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
		case KEY_PRESS_SPACE:
			//Penelope's flamethrower charge should decrease by 1
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			if (getDirection() == up)
			{
				for (int i = 0; i < 3; i++)
				{
					double flame_posx = current_x;
					double flame_posy = current_y + (i+1) * SPRITE_HEIGHT;
					if (getWorld()->containsObstacle(flame_posx, flame_posy))
						break;
					Flame* new_flame = new Flame(getWorld(), flame_posx, flame_posy, up);
					getWorld()->addintovector(new_flame);
				}
			}
			if (getDirection() == down)
			{
				for (int i = 0; i < 3; i++)
				{
					double flame_posx = current_x;
					double flame_posy = current_y - (i + 1) * SPRITE_HEIGHT;
					if (getWorld()->containsObstacle(flame_posx, flame_posy))
						break;
					Flame* new_flame = new Flame(getWorld(), flame_posx, flame_posy, up);
					getWorld()->addintovector(new_flame);
				}
			}
			if (getDirection() == left)
			{
				for (int i = 0; i < 3; i++)
				{
					double flame_posx = current_x - (i + 1)*SPRITE_WIDTH;
					double flame_posy = current_y;
					if (getWorld()->containsObstacle(flame_posx, flame_posy))
						break;
					Flame* new_flame = new Flame(getWorld(), flame_posx, flame_posy, up);
					getWorld()->addintovector(new_flame);
				}
			}
			if (getDirection() == right)
			{
				for (int i = 0; i < 3; i++)
				{
					double flame_posx = current_x + (i + 1)*SPRITE_WIDTH;
					double flame_posy = current_y;
					if (getWorld()->containsObstacle(flame_posx, flame_posy))
						break;
					Flame* new_flame = new Flame(getWorld(), flame_posx, flame_posy, up);
					getWorld()->addintovector(new_flame);
				}
			}

		//	{
		//		double flame_coord_x = current_x + SPRITE_WIDTH;
		//		for (int i = 0; i < 3; i++)
		//		{
		//			if (getWorld()->containsObstacle(flame_coord_x, current_y))
		//				break;
		//			Flame* x = new Flame(getWorld(), flame_coord_x, current_y, right);
		//			flame_coord_x += SPRITE_WIDTH;
		//		}
		//	}
			break;
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
	increasetickcount();
	Person::doSomething();		//increases m_infectioncount here
	if (getInfectionCount() == 500)
	{
		setStatus(false);
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
		Actor* new_Zombie;
		int whichZombie = randInt(1, 10);
		if (whichZombie > 3)
			new_Zombie = new DumbZombie(getWorld(), getX(), getY());
		else new_Zombie = new SmartZombie(getWorld(), getX(), getY());
		getWorld()->addintovector(new_Zombie);
		return;
	}
	if (gettickcount() % 2 == 0)
		return;

}

Zombie::Zombie(StudentWorld* sw, double x, double y)
	:MovingObjects(sw, IID_ZOMBIE, x, y)
{}

std::string Zombie::defineObjectType() const { return "ZOMBIE"; }

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

Wall::Wall(StudentWorld* sw, double x, double y)
	:Actor(sw, IID_WALL,x, y, right, 0)
{}

void Wall::doSomething() {}

bool Wall::blocksMovement() const { return true; }

std::string Wall::defineObjectType() const { return "WALL"; }

StillObjects::StillObjects(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:Actor(sw, imageID, x, y, dir, depth)
{}

bool StillObjects::blocksMovement() const { return false; }

Exit::Exit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_EXIT, x, y, right, 1)
{}

void Exit::doSomething()
{
	if (getWorld()->determineOverlapwithCitizen(getX(), getY()))
	{
		getWorld()->increaseScore(500);
		getWorld()->setOverlappedCitizentoDead(getX(), getY());
		getWorld()->playSound(SOUND_CITIZEN_SAVED);
	}
	if (getWorld()->determineOverlapwithPlayer(getX(), getY()))
	{
		if (getWorld()->getNumCitizensLeft() == 0)
			getWorld()->advanceToNextLevel();
	}
}

std::string Exit::defineObjectType() const { return "EXIT"; }

Pit::Pit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_PIT, x, y, right, 0)
{}

void Pit::doSomething() {}

std::string Pit::defineObjectType() const { return "PIT"; }

Goodie::Goodie(StudentWorld* sw, int imageID, double x, double y)
	:StillObjects(sw, imageID, x, y, right, 1)
{}

std::string Goodie::defineObjectType() const { return "GOODIE"; }

bool Goodie::canbeDamaged() const { return true; }

VaccineGoodie::VaccineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_VACCINE_GOODIE, x, y)
{}

void VaccineGoodie::doSomething() {}

GasCanGoodie::GasCanGoodie(StudentWorld* sw, double x, double y)
	:Goodie(sw, IID_GAS_CAN_GOODIE, x, y)
{}

void GasCanGoodie::doSomething() {}

LandmineGoodie::LandmineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_LANDMINE_GOODIE, x, y)
{}

void LandmineGoodie::doSomething() {}

Landmine::Landmine(StudentWorld* sw, double x, double y)
	: StillObjects(sw, IID_LANDMINE, x, y, right, 1)
{}

void Landmine::doSomething()
{}

bool Landmine::canbeDamaged() const { return true; }

std::string Landmine::defineObjectType() const { return "LANDMINE"; }

Projectile::Projectile(StudentWorld* sw, int imageID, double x, double y, int dir)
	:StillObjects(sw, imageID, x, y, dir, 0)
{}

Flame::Flame(StudentWorld* sw, double x, double y, int dir)
	: Projectile(sw, IID_FLAME, x, y, dir)
{}

void Flame::doSomething()
{
	increasetickcount();
	if (!getStatus()) return;
	else if (gettickcount() == 2)
	{
			setStatus(false);
			return;
	}
	else
	{
		getWorld()->getKilledbyFlame(getX(), getY());
	}
}

std::string Flame::defineObjectType() const { return "FLAME"; }

Vomit::Vomit(StudentWorld* sw, double x, double y, int dir)
	: Projectile(sw, IID_VOMIT, x, y, dir)
{
	getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
}

std::string Vomit::defineObjectType() const { return "VOMIT"; }

void Vomit::doSomething() {}

