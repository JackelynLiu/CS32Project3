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

bool Actor::canExit() const { return false; }

bool Actor::isGoodie() const { return false; }

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
	if (!getStatus())
		return;
	if (m_infectedstatus)
		m_infectioncount++;
}

bool Person::canbeInfected() const { return true; }

int Person::getInfectionCount() const { return m_infectioncount; }

void Person::setInfectionCount(int n) { m_infectioncount = n; }

bool Person::canExit() const { return true; }

bool Person::getInfectedStatus() const { return m_infectedstatus; }

void Person::setInfectedStatus(bool infected) { m_infectedstatus = infected; }

Penelope::Penelope(StudentWorld* sw, double x, double y)
	:Person(sw, IID_PLAYER, x, y), num_vaccines(0), num_landmines(0), num_flamecharges(0)
{}

void Penelope::doSomething()
{
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
				setInfectedStatus(false);
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
	getWorld()->increaseScore(-1000);
}

void Citizen::doSomething()
{
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

	double dist_p = getWorld()->distanceFromPenelope(current_x, current_y);
	double dist_z = getWorld()->distanceFromNearestZombie(current_x, current_y);

	double px = getWorld()->getPenelopexcoord();
	double py = getWorld()->getPenelopeycoord();

	if (dist_p <= dist_z && dist_p <= 80)
	{
		if (current_y == py)			//if Penelope is in the same row
		{
			if (px <= current_x)		//if Penelope is on the left
			{
				setDirection(left);
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x - 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x-2, current_y)))
				{
					moveTo(current_x - 2, current_y);
					return;
				}
			}
			else if (px >= current_x)
			{
				setDirection(right);
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x + 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x + 2, current_y)))
				{
					moveTo(current_x + 2, current_y);
					return;
				}
			}
		}
		else if (current_x == px)		//if Penelope is in the same column
		{
			if (py <= current_y)		//if Penelope is below
			{
				setDirection(down);
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y-2)))
				{
					moveTo(current_x, current_y - 2);
					return;
				}
			}
			else if (py >= current_y)
			{
				setDirection(up);
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y + 2)))
				{
					moveTo(current_x, current_y + 2);
					return;
				}
			}
		}
		else
		{
			Direction h = right;
			Direction v = up;
			Direction f = v;
			if (px <= current_x)
				h = left;
			if (py <= current_y)
				v = down;
			int choose = randInt(1, 2);
			if (choose == 1)
				f = h;
			else f = v;
			setDirection(f);
			switch (f)
			{
			case right:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x + 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x + 2, current_y)))
				{
					moveTo(current_x + 2, current_y);
					return;
				}
			case left:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x - 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x - 2, current_y)))
				{
					moveTo(current_x - 2, current_y);
					return;
				}
			case up:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y + 2)))
				{
					moveTo(current_x, current_y + 2);
					return;
				}
			case down:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y - 2)))
				{
					moveTo(current_x, current_y - 2);
					return;
				}
			default:
				break;
			}
			Direction f2 = f;
			if (f == up || f == down)
				f2 = h;
			//else f2 = v;
			setDirection(f2);
			switch (f2)
			{
			case right:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x + 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x + 2, current_y)))
				{
					moveTo(current_x + 2, current_y);
					return;
				}
			case left:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x - 2, current_y)) &&
					!(getWorld()->containsPlayer(current_x - 2, current_y)))
				{
					moveTo(current_x - 2, current_y);
					return;
				}
			case up:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y + 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y + 2)))
				{
					moveTo(current_x, current_y + 2);
					return;
				}
			case down:
				if (!(getWorld()->containsObstacle(current_x, current_y, current_x, current_y - 2)) &&
					!(getWorld()->containsPlayer(current_x, current_y - 2)))
				{
					moveTo(current_x, current_y - 2);
					return;
				}
			default:
				break;
			}
		}
	}
	else if (dist_z <= 80)
	{
		map<double, Direction> dist2dir;
		double distR, distL, distU, distD;
		double dist_max = dist_z;
		Direction dir_max = right;

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
		it--;

		dist_max = it->first;
		if (dist_max >= dist_z)
			dir_max = it->second;
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
	else return;
}

Zombie::Zombie(StudentWorld* sw, double x, double y)
	:MovingObjects(sw, IID_ZOMBIE, x, y), movement_plan(0)
{}

void Zombie::doSomething()
{
	increasetickcount();
	if (!getStatus()) return;
	if (gettickcount() % 2 == 0) return;
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
}

int Zombie::getmovementPlan() const { return movement_plan; }
void Zombie::setmovementPlan(int num) { movement_plan = num; }

SmartZombie::SmartZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void SmartZombie::changeStatus()
{
	Actor::changeStatus();
	getWorld()->increaseScore(2000);
}

void SmartZombie::doSomething()
{
	Zombie::doSomething();


}

DumbZombie::DumbZombie(StudentWorld* sw, double x, double y)
	:Zombie(sw, x, y)
{}

void DumbZombie::changeStatus()
{
	Actor::changeStatus();
	getWorld()->increaseScore(1000);
}

void DumbZombie::doSomething()	//carrying vaccine -- remember to implement
{
	Zombie::doSomething();
	
	if (getmovementPlan() == 0)
	{
		int r = randInt(3, 10);
		setmovementPlan(r);
		int d = randInt(1, 4);
		switch (d)
		{
		case 1:
			setDirection(right);
			break;
		case 2:
			setDirection(left);
			break;
		case 3:
			setDirection(down);
			break;
		case 4:
			setDirection(up);
			break;
		}
		setmovementPlan(r);
	}

	double dest_x = getX();
	double dest_y = getY();
	switch (getDirection())
	{
	case right:
		dest_x++;
		break;
	case left:
		dest_x--;
		break;
	case up:
		dest_y++;
		break;
	case down:
		dest_y--;
		break;
	}
	if (!getWorld()->containsObstacle(getX(), getY(), dest_x, dest_y) && !getWorld()->containsPlayer(dest_x, dest_y))
	{
		moveTo(dest_x, dest_y);
		setmovementPlan(getmovementPlan() - 1);
	}
	else setmovementPlan(0);
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
	if (getWorld()->determineOverlapwithCitizen(getX(), getY()))
	{
		getWorld()->increaseScore(500);
		getWorld()->playSound(SOUND_CITIZEN_SAVED);
	}
	if (getWorld()->determineOverlapwithPlayer(getX(), getY()))
	{
		if (getWorld()->getNumCitizensLeft() == 0)
			getWorld()->advanceToNextLevel();
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

bool Goodie::isGoodie() const { return true; }

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
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY(), right));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY(), right));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY(), right));
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY() - SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX(), getY() + SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, right));
	getWorld()->addintovector(new Flame(getWorld(), getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, right));
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

