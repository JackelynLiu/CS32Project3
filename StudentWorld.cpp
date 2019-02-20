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

//StudentWorld::~StudentWorld() { cleanUp(); }

int StudentWorld::init()
{
	Level lev(assetPath());
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr << "Cannot find level01.txt data file" << endl;
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
					break;
				case Level::dumb_zombie:
					break;
				case Level::player:
					m_player = new Penelope(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT, this);
					break;
				case Level::exit:
					break;
				case Level::wall:
					wallObjects.push_back(new Wall(level_x*SPRITE_WIDTH, level_y*SPRITE_HEIGHT));
					break;
				case Level::pit:
					break;
					// etc…
				}
			}
		}
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
	/*for (int i = 0; i < wallObjects.size(); i++)
	{
		delete wallObjects[i];
	}*/
	cout << "Completed clean up." << endl;
}

bool StudentWorld::containsWall(double x, double y)
{
	vector<Actor*>::iterator it;
	it = wallObjects.begin();

	while (it != wallObjects.end())
	{
		double lower_x = (*it)->getX();
		double lower_y = (*it)->getY();
		double upper_x = lower_x + SPRITE_WIDTH - 1;
		double upper_y = lower_y + SPRITE_HEIGHT - 1;
		if (lower_x <= x && x <= upper_x && lower_y <= y && y <= upper_y)
			return true;
		if (lower_x <= x + SPRITE_WIDTH - 1 && x + SPRITE_WIDTH - 1 <= upper_x &&
			lower_y <= y + SPRITE_HEIGHT - 1 && y + SPRITE_HEIGHT - 1 <= upper_y)
			return true;
		it++;
	}
	return false;
}