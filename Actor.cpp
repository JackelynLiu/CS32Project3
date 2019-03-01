// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"
#include <map>
using namespace std;

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth), current_world(sw), m_status(true), m_tickcount(0)
{}

bool Actor::getStatus() const { return m_status; }

void Actor::changeStatus() { m_status = false; }

int Actor::gettickcount() const { return m_tickcount; }

void Actor::increasetickcount() { m_tickcount++; }

bool Actor::canbeDamaged() const { return false; }

bool Actor::canbeInfected() const { return false; }

bool Actor::blocksFlame() const { return false; }

void Actor::useExitIfAppropriate() {};

void Actor::activateifAppropriate(Actor* a) {}

bool Actor::canbePickedUp() const { return false; }

bool Actor::triggersCitizens() const { return false; }

bool Actor::threatensCitizens() const { return false; }

bool Actor::isAt(double x, double y)
{
	if (getX() == x && getY() == y)
		return true;
	else return false;
}

bool Actor::canactivateLandmine() const { return false; }

MovingObjects::MovingObjects(StudentWorld* sw, int imageID, double x, double y)
	:Actor(sw, imageID, x, y, right, 0)
{}

bool MovingObjects::blocksMovement() const { return true; }

bool MovingObjects::canbeDamaged() const { return true; }

bool MovingObjects::canactivateLandmine() const { return true; }

Person::Person(StudentWorld* sw, int imageID, double x, double y)
	:MovingObjects(sw, imageID, x, y), m_infectioncount(0), m_infectedstatus(false)
{}

void Person::doSomething()
{
	if (m_infectedstatus)
		m_infectioncount++;
}

bool Person::canbeInfected() const { return true; }

int Person::getInfectionCount() const { return m_infectioncount; }

void Person::setInfectionCount(int n) { m_infectioncount = n; }

bool Person::getInfectedStatus() const { return m_infectedstatus; }

void Person::setInfectedStatus(bool infected) { m_infectedstatus = infected; }

Penelope::Penelope(StudentWorld* sw, double x, double y)
	:Person(sw, IID_PLAYER, x, y), num_vaccines(0), num_landmines(0), num_flamecharges(0)
{}

//void Penelope::useExitIfAppropriate()
//{
//	if (getWorld()->getNumCitizensLeft() == 0)
//	{
//		//getWorld()->completedLevel();
//	}
//}

bool Penelope::triggersCitizens() const { return true; }

void Penelope::doSomething()
{
	if (!getStatus())
		return;
	Person::doSomething();
	if (getInfectionCount() == 500)
	{
		changeStatus();
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
			if (!(getWorld()->containsObstacle(current_x, current_y, current_x - 4, current_y)))
				moveTo(current_x - 4, current_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!(getWorld()->containsObstacle(current_x, current_y, current_x + 4, current_y)))
				moveTo(current_x + 4, current_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 4)))
				moveTo(current_x, current_y - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 4)))
				moveTo(current_x, current_y + 4);
			break;
		case KEY_PRESS_SPACE:
		{
			if (num_flamecharges == 0)
				break;
			else num_flamecharges--;
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			if (getDirection() == up)
			{
				for (int i = 0; i < 3; i++)
				{
					double flame_posx = current_x;
					double flame_posy = current_y + (i + 1) * SPRITE_HEIGHT;
					if (getWorld()->containsObstacleforFlame(flame_posx, flame_posy))
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
					if (getWorld()->containsObstacleforFlame(flame_posx, flame_posy))
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
					if (getWorld()->containsObstacleforFlame(flame_posx, flame_posy))
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
					if (getWorld()->containsObstacleforFlame(flame_posx, flame_posy))
						break;
					Flame* new_flame = new Flame(getWorld(), flame_posx, flame_posy, up);
					getWorld()->addintovector(new_flame);
				}
			}
			break;
		}
		case KEY_PRESS_TAB:
		{
			if (num_landmines == 0)
				break;
			else num_landmines--;
			Landmine* new_landmine = new Landmine(getWorld(), current_x, current_y);
			getWorld()->addintovector(new_landmine);
			break;
		}
		case KEY_PRESS_ENTER:
		{
			if (num_vaccines == 0)
				break;
			else num_vaccines--;
			if (getInfectedStatus())
			{
				setInfectedStatus(false);
				setInfectionCount(0);
			}
			break;
		}
		default:
			return;
		}
	}
}

void Penelope::increaseVaccines()
{
	num_vaccines++;
}

void Penelope::increaseFlameCharges()
{
	num_flamecharges += 5;
}

void Penelope::increaseLandmines()
{
	num_landmines += 2;
}

int Penelope::getNumVaccines() const { return num_vaccines; }
int Penelope::getNumFlameCharges() const { return num_flamecharges; }
int Penelope::getNumLandmines() const{ return num_landmines; }



Citizen::Citizen(StudentWorld* sw, double x, double y)
	:Person(sw, IID_CITIZEN, x, y)
{}

void Citizen::changeStatus() 
{
	Actor::changeStatus();
	getWorld()->playSound(SOUND_CITIZEN_DIE);
	getWorld()->recordCitizenGone();
	getWorld()->increaseScore(-1000);
}

void Citizen::doSomething()
{
	if (!getStatus())
		return;
	increasetickcount();
	Person::doSomething();		//increases m_infectioncount here
	double current_x = getX();
	double current_y = getY();
	if (getInfectionCount() == 500)
	{
		changeStatus();
		getWorld()->playSound(SOUND_ZOMBIE_BORN);
		getWorld()->increaseScore(-1000);
		Actor* new_Zombie;
		int whichZombie = randInt(1, 10);
		if (whichZombie > 3)
			new_Zombie = new DumbZombie(getWorld(), current_x, current_y);
		else new_Zombie = new SmartZombie(getWorld(), current_x, current_y);
		getWorld()->addintovector(new_Zombie);
		return;
	}
	if (gettickcount() % 2 == 0)
		return;

	double trigger_x, trigger_y, trigger_dist = 0;
	bool threat = false;
	if (getWorld()->locateNearestCitizenTrigger(getX(), getY(), trigger_x, trigger_y, trigger_dist, threat))
	{
		if (threat)
		{
			map<double, Direction> dist2dir;
			double distR, distL, distU, distD;
			double dist_max = trigger_dist;
			Direction dir_max = getDirection();

			if (!getWorld()->containsObstacle(current_x, current_y, current_x + 2, current_y))
			{
				distR = getWorld()->distanceFromNearestZombie(current_x + 2, current_y);
				dist2dir[distR] = right;
			}
			if (!getWorld()->containsObstacle(current_x, current_y, current_x - 2, current_y))
			{
				distL = getWorld()->distanceFromNearestZombie(current_x - 2, current_y);
				dist2dir[distL] = left;
			}
			if (!getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 2))
			{
				distU = getWorld()->distanceFromNearestZombie(current_x, current_y + 2);
				dist2dir[distU] = up;
			}
			if (!getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 2))
			{
				distD = getWorld()->distanceFromNearestZombie(current_x, current_y - 2);
				dist2dir[distD] = down;
			}

			//map is in ascending order so last one is max
			map<double, Direction>::iterator it;
			it = dist2dir.end();
			if (dist2dir.empty()) return;
			it--;			//i cant decrement it??? pls fix
			dist_max = (*it).first;
			if (dist_max >= trigger_dist)
				dir_max = (*it).second;
			else return;

			setDirection(dir_max);
			switch (dir_max)
			{
			case right:
			{
				moveTo(current_x + 2, current_y);
				break;
			}
			case left:
			{
				moveTo(current_x - 2, current_y);
				break;
			}
			case up:
			{
				moveTo(current_x, current_y + 2);
				break;
			}
			case down:
			{
				moveTo(current_x, current_y - 2);
				break;
			}

			}
		
		}

		if (!threat)
		{
			if (trigger_x == current_x)		//if chosen trigger is in same column
			{
				if (trigger_y > current_y)		//if trigger is above
					setDirection(up);
				else setDirection(down);
			}
			else if (trigger_y == current_y)		//if chosen trigger is in same row
			{
				if (trigger_x > current_x)		//if trigger is to the right
					setDirection(right);
				else setDirection(left);
			}
			else
			{
				int r = randInt(1, 2);
				if (trigger_x < getX() && trigger_y < getY())
				{
					if (r == 1)
						setDirection(left);
					else setDirection(down);
				}
				else if (trigger_x < getX() && trigger_y > getY())
				{
					if (r == 1)
						setDirection(left);
					else setDirection(up);
				}
				else if (trigger_x > getX() && trigger_y < getY())
				{
					if (r == 1)
						setDirection(right);
					else setDirection(down);
				}
				else if (trigger_x > getX() && trigger_y > getY())
				{
					if (r == 1)
						setDirection(right);
					else setDirection(up);
				}

			}

			switch (getDirection())
			{
			case right:
			{
				if (!getWorld()->containsObstacle(current_x, current_y, current_x + 2, current_y) &&
					!getWorld()->containsPlayer(current_x + 2, current_y))
					moveTo(current_x + 2, current_y);
				break;
			}
			case left:
			{
				if (!getWorld()->containsObstacle(current_x, current_y, current_x - 2, current_y) &&
					!getWorld()->containsPlayer(current_x - 2, current_y))
					moveTo(current_x - 2, current_y);
				break;
			}
			case up:
			{
				if (!getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 2) &&
					!getWorld()->containsPlayer(current_x, current_y + 2))
					moveTo(current_x, current_y + 2);
				break;
			}
			case down:
			{
				if (!getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 2) &&
					!getWorld()->containsPlayer(current_x, current_y - 2))
					moveTo(current_x, current_y - 2);
				break;
			}

			}
		}
	}
	else return;
}

void Citizen::useExitIfAppropriate()
{
	Actor::changeStatus();
	getWorld()->playSound(SOUND_CITIZEN_SAVED);
	getWorld()->recordCitizenGone();
	getWorld()->increaseScore(500);
}

Zombie::Zombie(StudentWorld* sw, double x, double y)
	:MovingObjects(sw, IID_ZOMBIE, x, y), movement_plan(0)
{}

void Zombie::changeStatus()
{
	Actor::changeStatus();
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
}

bool Zombie::triggersCitizens() const { return true; }
bool Zombie::threatensCitizens() const { return true; }

void Zombie::doSomething()
{
	double vomit_x = 0;
	double vomit_y = 0;
	int r = randInt(1, 3);
	switch (getDirection())
	{
	case right:
	{
		vomit_x = getX() + SPRITE_WIDTH;
		vomit_y = getY();
		if (getWorld()->isZombieVomitTriggerAt(vomit_x, vomit_y))
		{
			if (r == 1)
			{
				getWorld()->addintovector(new Vomit(getWorld(), vomit_x, vomit_y, right));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			}
		}
		break;
	}
	case left:
	{
		vomit_x = getX() - SPRITE_WIDTH;
		vomit_y = getY();
		if (getWorld()->isZombieVomitTriggerAt(vomit_x, vomit_y))
		{
			if (r == 1)
			{
				getWorld()->addintovector(new Vomit(getWorld(), vomit_x, vomit_y, left));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			}
		}
		break;
	}
	case up:
	{
		vomit_x = getX();
		vomit_y = getY() + SPRITE_HEIGHT;
		if (getWorld()->isZombieVomitTriggerAt(vomit_x, vomit_y))
		{
			if (r == 1)
			{
				getWorld()->addintovector(new Vomit(getWorld(), vomit_x, vomit_y, up));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			}
		}
		break;
	}
	case down:
	{
		vomit_y = getX();
		vomit_y = getY() - SPRITE_HEIGHT;
		if (getWorld()->isZombieVomitTriggerAt(vomit_x, vomit_y))
		{
			if (r == 1)
			{
				getWorld()->addintovector(new Vomit(getWorld(), vomit_x, vomit_y, down));
				getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			}
		}
		break;
	}
	}

	if (movement_plan == 0)
	{
		int r = randInt(3, 10);
		movement_plan = r;
	}

}

void Zombie::setRandomDirection()
{
	int r = randInt(1, 4);
	switch (r)
	{
	case 1:
	{
		setDirection(right);
		break;
	}
	case 2:
	{
		setDirection(left);
		break;
	}
	case 3:
	{
		setDirection(up);
		break;
	}
	case 4:
	{
		setDirection(down);
		break;
	}
	default:
		break;
	}
}

void Zombie::moveinDirection()
{
	double dest_x = getX();
	double dest_y = getY();
	switch (getDirection())
	{
	case right:
	{
		dest_x++;
		break;
	}
	case left:
	{
		dest_x--;
		break;
	}
	case up:
	{
		dest_y++;
		break;
	}
	case down:
	{
		dest_y--;
		break;
	}
	}
	if (!getWorld()->containsObstacle(getX(), getY(), dest_x, dest_y) && !getWorld()->containsPlayer(dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		movement_plan--;
	}
	else movement_plan = 0;
}

SmartZombie::SmartZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void SmartZombie::changeStatus()
{
	Zombie::changeStatus();
	getWorld()->increaseScore(2000);
}

void SmartZombie::doSomething()
{
	increasetickcount();
	if (!getStatus()) return;
	if (gettickcount() % 2 == 0) return;
	Zombie::doSomething();
	
	double hx, hy, h_dist = 0;
	if (getWorld()->locateNearestVomitTrigger(getX(), getY(), hx, hy, h_dist))
	{
		if (hx == getX())		//chosen human is in same column as zombie
		{
			if (hy < getY())	//chosen human is lower than zombie
				setDirection(down);
			else setDirection(up);
		}
		else if (hy == getY())		//chosen human is in same row as zombie
		{
			if (hx < getX())	//chosen human is on the left of zombie
				setDirection(left);
			else setDirection(up);
		}
		else
		{
			int r = randInt(1, 2);
			if (hx < getX() && hy < getY())
			{
				if (r == 1)
					setDirection(left);
				else setDirection(down);
			}
			else if (hx < getX() && hy > getY())
			{
				if (r == 1)
					setDirection(left);
				else setDirection(up);
			}
			else if (hx > getX() && hy < getY())
			{
				if (r == 1)
					setDirection(right);
				else setDirection(down);
			}
			else if (hx > getX() && hy > getY())
			{
				if (r == 1)
					setDirection(right);
				else setDirection(up);
			}

		}
	}
	else setRandomDirection();
	moveinDirection();
	
}

DumbZombie::DumbZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void DumbZombie::changeStatus()
{
	Zombie::changeStatus();
	getWorld()->increaseScore(1000);
}

void DumbZombie::doSomething()	//carrying vaccine -- remember to implement
{
	increasetickcount();
	if (!getStatus()) return;
	if (gettickcount() % 2 == 0) return;
	Zombie::doSomething();
	setRandomDirection();
	moveinDirection();
}

Wall::Wall(StudentWorld* sw, double x, double y)
	:Actor(sw, IID_WALL,x, y, right, 0)
{}

void Wall::doSomething() {}

bool Wall::blocksMovement() const { return true; }

bool Wall::blocksFlame() const { return true; }

StillObjects::StillObjects(StudentWorld* sw, int imageID, double x, double y, int dir, int depth)
	:Actor(sw, imageID, x, y, dir, depth)
{}

bool StillObjects::blocksMovement() const { return false; }

Exit::Exit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_EXIT, x, y, right, 1)
{}

void Exit::doSomething()
{
	getWorld()->determineOverlapwithCitizen(getX(), getY());
	if (getWorld()->determineOverlapwithPlayer(getX(), getY()))
	{
		if (getWorld()->getNumCitizensLeft() == 0)
			getWorld()->completedLevel();
	}
}

bool Exit::blocksFlame() const { return true; }

Pit::Pit(StudentWorld* sw, double x, double y)
	:StillObjects(sw, IID_PIT, x, y, right, 0)
{}

void Pit::doSomething()
{
	if (getWorld()->determineOverlapwithPlayer(getX(), getY()))
	{
		getWorld()->setPenelopetoDead();
	}
	getWorld()->getKilledbyFlameorPit(getX(), getY());
}

Goodie::Goodie(StudentWorld* sw, int imageID, double x, double y)
	:StillObjects(sw, imageID, x, y, right, 1)
{}

void Goodie::doSomething()
{
	if (!getStatus()) return;
	if (getWorld()->determineOverlapwithPlayer(getX(), getY()))
	{
		changeStatus();
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(50);
		getWorld()->pickupGoodies(getX(), getY());
		
	}
}

bool Goodie::canbeDamaged() const { return true; }

bool Goodie::canbePickedUp() const { return true; }

VaccineGoodie::VaccineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_VACCINE_GOODIE, x, y)
{}

void VaccineGoodie::doSomething()
{
	Goodie::doSomething();
}

void VaccineGoodie::pickup(Penelope *p)
{
	p->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* sw, double x, double y)
	:Goodie(sw, IID_GAS_CAN_GOODIE, x, y)
{}

void GasCanGoodie::doSomething()
{
	Goodie::doSomething();
}

void GasCanGoodie::pickup(Penelope* p)
{
	p->increaseFlameCharges();
}

LandmineGoodie::LandmineGoodie(StudentWorld* sw, double x, double y)
	: Goodie(sw, IID_LANDMINE_GOODIE, x, y)
{}

void LandmineGoodie::doSomething()
{
	Goodie::doSomething();
}

void LandmineGoodie::pickup(Penelope * p)
{
	p->increaseLandmines();
}

Landmine::Landmine(StudentWorld* sw, double x, double y)
	: StillObjects(sw, IID_LANDMINE, x, y, right, 1), m_active(false)
{}

void Landmine::doSomething()
{
	increasetickcount();
	if (!getStatus()) return;
	if (gettickcount() == 30)
	{
		m_active = true;
	}
	if (m_active)
		if (getWorld()->isLandmineTriggered(getX(), getY()))
			explode();
}

void Landmine::explode()
{
	changeStatus();
	getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY(), up));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY(), up));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY(), up));
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY() - SPRITE_HEIGHT, up));
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY() + SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up));
	getWorld()->addintovector(new Pit(getWorld(), getX(), getY()));
}

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
			changeStatus();
			return;
	}
	else
	{
		getWorld()->getKilledbyFlameorPit(getX(), getY());
	}
}

bool Flame::canactivateLandmine() const { return true; }

Vomit::Vomit(StudentWorld* sw, double x, double y, int dir)
	: Projectile(sw, IID_VOMIT, x, y, dir)
{
	getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
}

void Vomit::doSomething()
{
	increasetickcount();
	if (!getStatus())
		return;
	else if (gettickcount() == 2)
	{
		changeStatus();
		return;
	}
	else
		getWorld()->infecteverything(getX(), getY());
}

