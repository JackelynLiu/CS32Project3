#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}

StudentWorld::~StudentWorld() { cleanUp(); }

int StudentWorld::init()
{
	num_alivecitizens = 0;
	level_completed = false;

	ostringstream levelstring;

	levelstring << "level";
	int n = getLevel();
	levelstring.fill('0');
	levelstring << setw(2) << n;
	levelstring << ".txt";

	Level lev(assetPath());
	string levelFile = levelstring.str();
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found || getLevel() == 100)
	{
		cerr << "Cannot find " << levelFile << " data file" << endl;
		return GWSTATUS_PLAYER_WON;
	}
	else if (result == Level::load_fail_bad_format)
	{
		cerr << "Your level was improperly formatted" << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int level_x = 0; level_x < LEVEL_WIDTH; level_x++)
		{
			for (int level_y = 0; level_y < LEVEL_HEIGHT; level_y++)
			{
				int coord_x = level_x * SPRITE_WIDTH;
				int coord_y = level_y * SPRITE_HEIGHT;
				Level::MazeEntry ge = lev.getContentsOf(level_x, level_y); // level_x=5, level_y=10
				switch (ge) // so x=80 and y=160
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
					gameObjects.push_back(new SmartZombie(this, coord_x, coord_y));
					break;
				case Level::dumb_zombie:
					gameObjects.push_back(new DumbZombie(this, coord_x, coord_y));
					break;
				case Level::player:
					m_player = new Penelope(this, coord_x, coord_y);
					break;
				case Level::citizen:
					gameObjects.push_back(new Citizen(this, coord_x, coord_y));
					num_alivecitizens++;
					break;
				case Level::exit:
					gameObjects.push_back(new Exit(this, coord_x, coord_y));
					break;
				case Level::wall:
					gameObjects.push_back(new Wall(this, coord_x, coord_y));
					break;
				case Level::pit:
					gameObjects.push_back(new Pit(this, coord_x, coord_y));
					break;
				case Level::gas_can_goodie:
					gameObjects.push_back(new GasCanGoodie(this, coord_x, coord_y));
					break;
				case Level::landmine_goodie:
					gameObjects.push_back(new LandmineGoodie(this, coord_x, coord_y));
					break;
				case Level::vaccine_goodie:
					gameObjects.push_back(new VaccineGoodie(this, coord_x, coord_y));
					break;
				}
			}
		}

		ostringstream oss;
		oss << "Score: " << getScore() << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Vaccines: " << m_player->getNumVaccines() <<
			"  Flames: " << m_player->getNumFlameCharges() << "  Mines: " << m_player->getNumLandmines() << "  Infected: " << m_player->getInfectionCount();
		string currentstats = oss.str();
		setGameStatText(currentstats);
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_player->doSomething();
	for (int i = 0; i < gameObjects.size(); i++)
		gameObjects[i]->doSomething();
	if (!m_player->getStatus()) return GWSTATUS_PLAYER_DIED;
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end();)
	{
		if (!(*it)->getStatus())
		{
			delete *it;
			it = gameObjects.erase(it);
		}
		else it++;
	}

	if (!m_player->getStatus())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (level_completed)
		return GWSTATUS_FINISHED_LEVEL;

	ostringstream oss;
	oss << "Score: ";
	if (getScore() >= 0)
	{
		oss.fill('0');
		oss << setw(6) << getScore();
	}
	else
	{
		oss << "-";
		oss.fill('0');
		oss << setw(5) << -getScore();
	}
	
	oss << "  Level: " << getLevel() << "  Lives: " << getLives() << "  Vaccines: " << m_player->getNumVaccines() <<
		"  Flames: " << m_player->getNumFlameCharges() << "  Mines: " << m_player->getNumLandmines() << "  Infected: " << m_player->getInfectionCount();
	string currentstats = oss.str();
	setGameStatText(currentstats);
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //decLives();
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	m_player = nullptr;
	cout << "deleted player" << endl;
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); )
	{
		delete (*it);
		it = gameObjects.erase(it);
	}
	cout << "vector size: " << gameObjects.size() << endl;
	cout << "Completed clean up." << endl;
}

void StudentWorld::addintovector(Actor* a)
{
	gameObjects.push_back(a);
}

bool StudentWorld::containsObstacle(double cur_x, double cur_y, double x, double y)
{
	vector<Actor*>::iterator it;
	it = gameObjects.begin();

	while (it != gameObjects.end())
	{
		if ((*it)->blocksMovement() && !(*it)->isAt(cur_x, cur_y))
		{
			double left_x = (*it)->getX();
			double lower_y = (*it)->getY();
			double right_x = left_x + SPRITE_WIDTH - 1;
			double upper_y = lower_y + SPRITE_HEIGHT - 1;
			if (left_x <= x && x <= right_x &&
				lower_y <= y && y <= upper_y)
				return true;
			if (left_x <= x && x <= right_x &&
				lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
				return true;
			if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
				lower_y <= y && y <= upper_y)
				return true;
			if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
				lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
				return true;
		}
		it++;
	}
	return false;
}

bool StudentWorld::containsPlayer(double x, double y)
{
	double left_x = m_player->getX();
	double lower_y = m_player->getY();
	double right_x = left_x + SPRITE_WIDTH - 1;
	double upper_y = lower_y + SPRITE_HEIGHT - 1;
	if (left_x <= x && x <= right_x &&
		lower_y <= y && y <= upper_y)
		return true;
	if (left_x <= x && x <= right_x &&
		lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
		return true;
	if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
		lower_y <= y && y <= upper_y)
		return true;
	if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
		lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
		return true;
	return false;
}

bool StudentWorld::containsObstacleforFlame(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->blocksFlame())
		{
			double left_x = (*it)->getX();
			double lower_y = (*it)->getY();
			double right_x = left_x + SPRITE_WIDTH - 1;
			double upper_y = lower_y + SPRITE_HEIGHT - 1;
			if (left_x <= x && x <= right_x &&
				lower_y <= y && y <= upper_y)
				return true;
			if (left_x <= x && x <= right_x &&
				lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
				return true;
			if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
				lower_y <= y && y <= upper_y)
				return true;
			if (left_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= right_x &&
				lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
				return true;
		}
	}
	return false;
}

bool StudentWorld::determineOverlapwithPlayer(double x, double y)
{
	double dist_x = m_player->getX() - x;
	double dist_y = m_player->getY() - y;
	if ((dist_x*dist_x) + (dist_y * dist_y) <= 100) return true;
	else return false;
}

bool StudentWorld::determineOverlapwithCitizen(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->canbeInfected())
		{
			double dist_x = (*it)->getX() - x;
			double dist_y = (*it)->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
			{
				(*it)->useExitIfAppropriate();
				return true;
			}
		}
	}
	return false;
}


void StudentWorld::setPenelopetoDead()
{
	m_player->changeStatus();
}

int StudentWorld::getNumCitizensLeft() const { return num_alivecitizens; }

void StudentWorld::getKilledbyFlameorPit(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->canbeDamaged())
		{
			double dist_x = (*it)->getX() - x;
			double dist_y = (*it)->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
			{
				(*it)->changeStatus();
			}
		}
	}
}

void StudentWorld::infecteverything(double x, double y)
{
	if (determineOverlapwithPlayer(x, y))
	{
		m_player->setInfectedStatus(true);
	}
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->canbeInfected())
		{
			double dist_x = (*it)->getX() - x;
			double dist_y = (*it)->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
			{
				static_cast<Person*>(*it)->setInfectedStatus(true);
				if (!static_cast<Person*>(*it)->getInfectedStatus())
					playSound(SOUND_CITIZEN_INFECTED);
			}
		}
	}
}

//write a general find distance function

double StudentWorld::distanceFromPenelope(double x, double y) const
{
	double dist_x = m_player->getX() - x;
	double dist_y = m_player->getY() - y;
	double total = sqrt((dist_x*dist_x) + (dist_y * dist_y));
	return total;
}

double StudentWorld::distanceFromNearestZombie(double x, double y)
{
	double min = 40000000;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		double temp_dist = 0;
		if (gameObjects[i]->canbeDamaged() && !(gameObjects[i]->canbeInfected()))
		{
			double dist_x = gameObjects[i]->getX() - x;
			double dist_y = gameObjects[i]->getY() - y;
			temp_dist = sqrt((dist_x*dist_x) + (dist_y * dist_y));
			if (temp_dist < min)
				min = temp_dist;
		}
	}
	return min;
}

void StudentWorld::pickupGoodies(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->isAt(x, y) && (*it)->canbePickedUp())
			(*it)->pickup(m_player);
	}
}

bool StudentWorld::isLandmineTriggered(double x, double y)
{
	if (determineOverlapwithPlayer(x, y)) return true;
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->canactivateLandmine())
		{
			double dist_x = (*it)->getX() - x;
			double dist_y = (*it)->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isZombieVomitTriggerAt(double x, double y)
{
	if (determineOverlapwithPlayer(x, y)) return true;
	for (int i = 0; i != gameObjects.size(); i++)
	{
		if (gameObjects[i]->canbeInfected())
		{
			double dist_x = gameObjects[i]->getX() - x;
			double dist_y = gameObjects[i]->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
				return true;
		}
	}
	return false;
}

bool StudentWorld::locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance)
{
	double dist_p = distanceFromPenelope(x, y);
	double dist_h = 400000000;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->canbeInfected())
		{
			double dist_x = gameObjects[i]->getX() - x;
			double dist_y = gameObjects[i]->getY() - y;
			double temp_dist = sqrt((dist_x*dist_x) + (dist_y * dist_y));
			if (temp_dist < dist_h)
			{
				dist_h = temp_dist;
				otherX = gameObjects[i]->getX();
				otherY = gameObjects[i]->getY();
			}
		}
	}
	if (dist_p < dist_h)
	{
		distance = dist_p;
		otherX = m_player->getX();
		otherY = m_player->getY();
	}
	else distance = dist_h;

	if (distance <= 80) return true;
	else return false;
}

bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const
{
	double dist_p = distanceFromPenelope(x, y);
	double tx, ty, t_dist = 0;
	if (locateNearestCitizenThreat(x, y, tx, ty, t_dist))
	{
		if (dist_p > t_dist)
		{
			isThreat = true;
			otherX = tx;
			otherY = ty;
			distance = t_dist;
			return true;
		}
		else
		{
			distance = dist_p;
			otherX = m_player->getX();
			otherY = m_player->getY();
		}

	}
	else
	{
		distance = dist_p;
		otherX = m_player->getX();
		otherY = m_player->getY();
	}

	if (distance <= 80) return true;
	else return false;

}

bool StudentWorld::locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const
{
	double dist_z = 400000000;
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->threatensCitizens())
		{
			double dist_x = gameObjects[i]->getX() - x;
			double dist_y = gameObjects[i]->getY() - y;
			double temp_dist = sqrt((dist_x*dist_x) + (dist_y * dist_y));
			if (temp_dist < dist_z)
			{
				dist_z = temp_dist;
				otherX = gameObjects[i]->getX();
				otherY = gameObjects[i]->getY();
			}
		}
	}
	distance = dist_z;
	if (distance <= 80)
		return true;
	else return false;
}

void StudentWorld::recordCitizenGone()
{
	num_alivecitizens--;
}

void StudentWorld::completedLevel()
{
	level_completed = true;
	playSound(SOUND_LEVEL_FINISHED);
}

bool StudentWorld::overlapsWithAnything(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		double dist_x = (*it)->getX() - x;
		double dist_y = (*it)->getY() - y;
		if (dist_x*dist_x + dist_y * dist_y <= 100)
			return true;
	}
	return false;
}