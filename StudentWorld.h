#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

class Actor;
class Penelope;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
	~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool containsObstacle(double x, double y);
	bool determineOverlap(double x, double y);

private:
	std::vector<Actor*> gameObjects;
	Penelope* m_player;
};

#endif // STUDENTWORLD_H_
