#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	std::vector<Actor*> &getActors();
	
	NachenBlaster* &getBlaster();
	void updateDisplayText();
	void increaseDeadAliens();
	~StudentWorld();
	void decreaseCurrAliens();

private:
	std::vector<Actor*> m_actors;
	NachenBlaster *m_ShipDiesAlot;
	int m_numOfAliensKilled;
	int m_numOfCurrAliens;
	int toComplete()
	{
		return 6 + (4 * getLevel());
	}
	int remainingShips()
	{
		return toComplete() - m_numOfAliensKilled;
	}
	int maxAlienNum()
	{
		return 4 + (.5 * getLevel());
	}


};

#endif // STUDENTWORLD_H_
