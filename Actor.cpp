// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth)
{
}

Penelope::Penelope(double x, double y, StudentWorld* sw)
	:Actor(IID_PLAYER, x, y, right, 0), studw(sw)
{
}

//Penelope::~Penelope()
//{
//	delete this;
//}

void Penelope::doSomething()
{
	if (studw->getLives() == 0)
		return;
	int ch;
	if (studw->getKey(ch))
	{
		double current_x = getX();
		double current_y = getY();
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (!(studw->containsObstacle(current_x - 4, current_y)))
				moveTo(current_x - 4, current_y);
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!(studw->containsObstacle(current_x + 4, current_y)))
				moveTo(current_x + 4, current_y);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!(studw->containsObstacle(current_x, current_y - 4)))
				moveTo(current_x, current_y - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!(studw->containsObstacle(current_x, current_y + 4)))
				moveTo(current_x, current_y + 4);
			break;
		/*case KEY_PRESS_SPACE:
			break*/
		default:
			return;
		}
	}
}

bool Penelope::blocksMovement() { return true; }

Wall::Wall(double x, double y)
	:Actor(IID_WALL,x, y, right, 0)
{

}

//Wall::~Wall() {}

void Wall::doSomething() {}

bool Wall::blocksMovement() { return true; }

Exit::Exit(double x, double y)
	:Actor(IID_EXIT, x, y, right, 0)
{}

void Exit::doSomething() {}
bool Exit::blocksMovement() { return false; }
