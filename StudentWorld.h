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
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	bool containsWall(double x, double y);

private:
	std::vector<Actor*> wallObjects;
	Penelope* m_player;
};

#endif // STUDENTWORLD_H_
