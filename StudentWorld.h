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

	bool containsObstacle(double cur_x, double cur_y, double x, double y);
	bool containsPlayer(double x, double y);
	bool determineOverlapwithPlayer(double x, double y);
	bool determineOverlapwithCitizen(double x, double y);
	void setPenelopetoDead();
	int getNumCitizensLeft() const;
	void getKilledbyFlameorPit(double x, double y);
	bool containsObstacleforFlame(double x, double y);
	void infecteverything(double x, double y);
	double distanceFromPenelope(double x, double y) const;
	double distanceFromNearestZombie(double x, double y);
	bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
	bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
	bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
	double getPenelopexcoord() const; //stop using
	double getPenelopeycoord() const; //stop using
	//int whattofollow(double x, double y);
	//Direction whichdirectionshouldCitizengo(double x, double y);
	bool isLandmineTriggered(double x, double y);

	void pickupGoodies(double x, double y);

	bool isZombieVomitTriggerAt(double x, double y);

	void recordCitizenGone();
	//void recordLevelFinishedIfAllCitizensGone();
	//bool isMovingObjectBlockedAt(double x, double y) const;

private:
	std::vector<Actor*> gameObjects;
	Penelope* m_player;
	int num_alivecitizens;
};

#endif // STUDENTWORLD_H_
