#include "StudentWorld.h"
#include "GameConstants.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw
#include <cmath>
using namespace std;
//vector<Actor*> m_actors;
GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


//*********************************************************************************************
//methods for returning actors to give other classes access to student world's private members*
//*********************************************************************************************
vector <Actor*> &StudentWorld::getActors()
{
	return m_actors;
}

NachenBlaster *&StudentWorld::getBlaster()
{
	return m_ShipDiesAlot;
}

StudentWorld::StudentWorld(string assetDir)
	: GameWorld(assetDir)
{

}
/*All new alien ships must start at x=VIEW_WIDTH-1, and a random y
between 0 and VIEW_HEIGHT-1, inclusive.*/


int StudentWorld::init()
{
	m_numOfAliensKilled = 0;
	m_numOfCurrAliens = 0;
	m_ShipDiesAlot = new NachenBlaster(this);
	//m_actors.push_back(m_ShipDiesAlot);
	for (int x = 0; x != 30; x++)
	{
		double sSize = (randInt(5, 50));
		//initilize all stars
		m_actors.push_back(new Stars(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), sSize / 100, this));
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{

	// This code is here merely to allow the game to build, run, and terminate after you hit enter.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	if (m_ShipDiesAlot->stillAlive())
		m_ShipDiesAlot->doSomething();
	vector <Actor*>::iterator it = m_actors.begin();
	while (it != m_actors.end())
	{
		if ((*it)->stillAlive())
		{
			(*it)->doSomething();
			if (!m_ShipDiesAlot->stillAlive())
			{
				decLives();
				cleanUp();
				return GWSTATUS_PLAYER_DIED;
			}
		}
		it++;
	}
	if (!m_ShipDiesAlot->stillAlive())
	{
		decLives();
		cleanUp();
		return GWSTATUS_PLAYER_DIED;
	}
	if (remainingShips() <= 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		cleanUp();
		return GWSTATUS_FINISHED_LEVEL;

	}
	//clean up dead actors

	it = m_actors.begin();
	while (it != m_actors.end())
	{
		if (!(*it)->stillAlive()) //if actor is dead delete it
		{
			delete *it;
			it = m_actors.erase(it);
		}
		else
			it++;
	}
	//1 in 15 chance to create a new star
	/*There is a one in fifteen chance that you will introduce a new star into the game
on the far right side of the screen (at x=VIEW_WIDTH-1). Each such star will
have a random y coordinate between [0, VIEW_HEIGHT). The size of each new
star must also be determined randomly, and must be between .05 and .5 units in
size.*/
	double sSize = (randInt(5, 50));
	if(randInt(1,15) == 1)
		m_actors.push_back(new Stars(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), sSize / 100, this));

	
	if (m_numOfCurrAliens < min(maxAlienNum(), remainingShips()))
	{
		int s1 = 60, s2 = 20 + getLevel() * 5, s3 = 5 + getLevel() * 10, s;
		s = s1 + s2 + s3;
		int alien = randInt(1, s);

		if (alien <= s1)
		{
			Actor* m_Smallgon = new Smallgon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this, (5 * (1 + (getLevel() - 1) * .1)));
			m_actors.push_back(m_Smallgon);

		}
		else if (alien <= (s1+s2) && alien > s1)
		{
			Actor* m_Smoregon = new Smoregon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this, (5 * (1 + (getLevel() - 1)*.1)));
			m_actors.push_back(m_Smoregon);
		}
		else
		{
			Actor* m_snag = new Snagglegon(VIEW_WIDTH - 1, randInt(0, VIEW_HEIGHT - 1), this, (10 * (1 + (getLevel() - 1) * .1)));
			m_actors.push_back(m_snag);
		}
		m_numOfCurrAliens++;
	}
	

	updateDisplayText();
	if (!getBlaster()->stillAlive())
	{
		return GWSTATUS_PLAYER_DIED;
	}
	else
	{
		return GWSTATUS_CONTINUE_GAME;

	}

    //decLives();
    //return GWSTATUS_PLAYER_DIED;
	
}

void StudentWorld::cleanUp()
{
	delete m_ShipDiesAlot;
	m_ShipDiesAlot = nullptr;

	vector <Actor*>::iterator it;
	it = m_actors.begin();
	while (it != m_actors.end())
	{
		delete *it;
		it++;
		//m_actors[x] = nullptr;
	}
	m_actors.clear();
	//delete m_ShipDiesAlot;
}
void StudentWorld::increaseDeadAliens()
{
	m_numOfAliensKilled++;
}

void StudentWorld::decreaseCurrAliens()
{
	m_numOfCurrAliens--;
}

void StudentWorld::updateDisplayText()
{
	ostringstream live, hp, score, cabbage, torp, getlev;
	cabbage.setf(ios::fixed);
	cabbage.precision(2);
	int lives = getLives();
	int scores = getScore();
	score << scores;
	string sc = score.str();
	int cabbages = (10.0 / 3.0) * (getBlaster()->getEnergyPoints());
	cabbage << cabbages;
	string cab = cabbage.str();
	int torpos = getBlaster()->getTorp();
	torp << torpos;
	string to = torp.str();
	live << lives;
	string s = live.str();
	int heal = 2*getBlaster()->getHealthPoints();
	hp << heal;
	string he = hp.str();
	int level = getLevel();
	getlev << level;
	string le = getlev.str();

	setGameStatText("Lives:  " + s + " Health:  " + he + "% Score:  " + sc + " Level:  " + le + " Cabbage:  " + cab + "% Torpedoes:  " + to);
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}
