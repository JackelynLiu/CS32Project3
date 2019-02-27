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
	virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

	void addintovector(Actor* a);

	bool containsObstacle(double x, double y);
	bool containsPlayer(double x, double y);
	bool determineOverlapwithPlayer(double x, double y);
	bool determineOverlapwithCitizen(double x, double y);
	//bool determineOverlapwithZombie(double x, double y);
	void setPenelopetoDead();
	//void setOverlappedZombietoDead(double x, double y);
	int getNumCitizensLeft() const;
	void getKilledbyFlameorPit(double x, double y);
	bool containsObstacleforFlame(double x, double y);
	void infecteverything(double x, double y);
	double distanceFromPenelope(double x, double y);
	double distanceFromNearestZombie(double x, double y);
	double getPenelopexcoord() const; //stop using
	double getPenelopeycoord() const; //stop using
	int whattofollow(double x, double y);

	void pickupGoodies(double x, double y);

	//void recordCitizenGone();
	//void recordLevelFinishedIfAllCitizensGone();
	//bool isMovingObjectBlockedAt(double x, double y) const;

private:
	std::vector<Actor*> gameObjects;
	Penelope* m_player;
	int num_alivecitizens;
};

#endif // STUDENTWORLD_H_
