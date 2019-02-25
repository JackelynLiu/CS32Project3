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
	void addintovector(Actor* a);
	bool containsObstacle(double x, double y);
	bool ExitOverlapwithPlayer(double x, double y);
	bool ExitOverlapwithCitizen(double x, double y);
	//bool determineOverlapwithZombie(double x, double y);
	void setPenelopetoDead();
	//void setOverlappedZombietoDead(double x, double y);
	int getNumCitizensLeft() const;
	void getKilledbyFlame(double x, double y);
	bool containsObstacleforFlame(double x, double y);
	void infecteverything(double x, double y);

private:
	std::vector<Actor*> gameObjects;
	Penelope* m_player;
	int num_alivecitizens;
	int num_vaccines;
	int num_landmines;
	int num_gascans;
};

#endif // STUDENTWORLD_H_
