// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(int imageID, double x, double y, int dir, int depth)
	:GraphObject(imageID, x, y, dir, depth)
{
}

Penelope::Penelope(double x, double y, StudentWorld* sw)
	: Actor(IID_PLAYER, x, y, right, 0)
{
	studw = sw;
}

Penelope::~Penelope()
{
	delete this;
}

void Penelope::doSomething()
{
	if (studw->getLives() == 0)
		return;
	int ch;
	if (studw->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(left);
			moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			moveTo(getX(), getY() - 4);
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			moveTo(getX(), getY() + 4);
			break;
		/*case KEY_PRESS_SPACE:
			break*/
		default:
			return;
		}
	}
}


Wall::Wall(double x, double y)
	:Actor(IID_WALL,x, y, right, 0)
{

}

Wall::~Wall() {}

void Wall::doSomething() {}

