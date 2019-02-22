#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
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
	Level lev(assetPath());
	string levelFile = "level03.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level03.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr << "Your level was improperly formatted" << endl;
	else if (result == Level::load_success)
	{
		cerr << "Successfully loaded level" << endl;
		for (int level_x = 0; level_x < LEVEL_WIDTH; level_x++)
		{
			for (int level_y = 0; level_y < LEVEL_HEIGHT; level_y++)
			{
				Level::MazeEntry ge = lev.getContentsOf(level_x, level_y); // level_x=5, level_y=10
				switch (ge) // so x=80 and y=160
				{
				case Level::empty:
					break;
				case Level::smart_zombie:
					gameObjects.push_back(new SmartZombie(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					break;
				case Level::dumb_zombie:
					gameObjects.push_back(new DumbZombie(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					break;
				case Level::player:
					m_player = new Penelope(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT);
					break;
				case Level::citizen:
					gameObjects.push_back(new Citizen(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					num_alivecitizens++;
					break;
				case Level::exit:
					gameObjects.push_back(new Exit(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					break;
				case Level::wall:
					gameObjects.push_back(new Wall(this, level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					break;
				case Level::pit:
					break;
					// etc�
				}
			}
		}
		setGameStatText("Score: 0   Level: 1   Lives: 3   Vacc: 0   Flames: 0   Mines: 0   Infected: 0");
	}
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_player->doSomething();
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

bool StudentWorld::determineOverlap(double x, double y)
{

	return false;
}