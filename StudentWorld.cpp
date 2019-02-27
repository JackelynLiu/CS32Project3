#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <iostream>
#include <sstream>
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
	Level lev(assetPath());
	string levelFile = "level04.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level04.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
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

bool StudentWorld::containsObstacle(double x, double y)
{
	vector<Actor*>::iterator it;
	it = gameObjects.begin();

	while (it != gameObjects.end())
	{
		if ((*it)->blocksMovement())
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
		if ((*it)->defineObjectType() == "CITIZEN")
		{
			double dist_x = (*it)->getX() - x;
			double dist_y = (*it)->getY() - y;
			if (dist_x*dist_x + dist_y * dist_y <= 100)
			{
				(*it)->setStatus(false);
				num_alivecitizens--;
				return true;
			}
		}
	}
	return false;
}

//bool StudentWorld::determineOverlapwithZombie(double x, double y)
//{
//	vector<Actor*>::iterator it;
//	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
//	{
//		if ((*it)->defineObjectType() == "ZOMBIE")
//		{
//			double dist_x = (*it)->getX() - x;
//			double dist_y = (*it)->getY() - y;
//			if (dist_x*dist_x + dist_y * dist_y <= 100)
//			{
//				(*it)->setStatus(false);
//				return true;
//			}
//		}
//	}
//	return false;
//}

void StudentWorld::setPenelopetoDead()
{
	m_player->setStatus(false);
	playSound(SOUND_PLAYER_DIE);
}

//void StudentWorld::setOverlappedZombietoDead(double x, double y)
//{
//	vector<Actor*>::iterator it;
//	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
//	{
//		if ((*it)->defineObjectType() == "ZOMBIE")
//		{
//			double dist_x = (*it)->getX() - x;
//			double dist_y = (*it)->getY() - y;
//			if (dist_x*dist_x + dist_y * dist_y <= 100)
//			{
//				(*it)->setStatus(false);
//				return;
//			}
//		}
//	}
//}

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
				(*it)->setStatus(false);
				if ((*it)->defineObjectType() == "CITIZEN")
				{
					increaseScore(-1000);
					cerr << "killed citizen";
				}
				else if ((*it)->defineObjectType() == "SMARTZOMBIE")
					increaseScore(2000);
				else if ((*it)->defineObjectType() == "DUMBZOMBIE")
					increaseScore(1000);

			}
		}
	}
}

void StudentWorld::infecteverything(double x, double y)
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
				(*it)->setInfectedStatus(true);
			}
		}
	}
}

double StudentWorld::distanceFromPenelope(double x, double y)
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
		if (gameObjects[i]->defineObjectType() == "ZOMBIE")
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

double StudentWorld::getPenelopexcoord() const { return m_player->getX(); }
double StudentWorld::getPenelopeycoord() const { return m_player->getY(); }

int StudentWorld::whattofollow(double x, double y)
{
	double dist_p = distanceFromPenelope(x, y);
	double dist_z = distanceFromNearestZombie(x, y);
	if (dist_p <= dist_z && dist_p <= 80)
		return 1;
	else return 2;
}

void StudentWorld::pickupGoodies(double x, double y)
{
	vector<Actor*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		if ((*it)->isAt(x, y) && (*it)->isGoodie())
			(*it)->pickup(m_player);
	}
	std::cerr << m_player->getNumVaccines();
	std::cerr << m_player->getNumFlameCharges();
	std::cerr << m_player->getNumLandmines();
}