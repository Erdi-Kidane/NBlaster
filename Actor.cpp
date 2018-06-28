#include "Actor.h"
#include "StudentWorld.h"
#include <cassert>
#include <cmath>
using namespace std;
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//help function to find distance between two objects
double euclidian_dist(double x1, double y1, double x2, double y2)
{
	double x = x1 - x2;
	double y = y1 - y2;
	double distance;
	distance = pow(x, 2) + pow(y, 2);
	distance = sqrt(distance);
	return distance;
}
bool collisionDetected(const Actor &A, const Actor &B)
{
	if ((euclidian_dist(A.getX(), A.getY(), B.getX(), B.getY())) < (.75 * (A.getRadius() + B.getRadius())))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//**********************************************************************//
//						Actor Implementations						   //
//********************************************************************//
Actor::Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld *World, bool isAlien, bool isHuman,
	bool isProjectile, bool isGoodie) : GraphObject(imageID, startX, startY, dir, size, depth), m_isAlien(isAlien), m_isHuman(isHuman),
	m_isProjectile(isProjectile), m_isGoodie(isGoodie)
{
	m_getWorld = World;
	m_alive = true;
}
Actor::~Actor() 
{}
int Actor::getDamageValue() const
{
	return 0;
}
void Actor::collided()
{
	return;
}
void Actor::lostHealth(int damage)
{
	return;
}
bool Actor::getAlien() const
{
	return m_isAlien;
}
bool Actor::getHuman() const
{
	return m_isHuman;
}
bool Actor::getProjectile() const
{
	return m_isProjectile;
}
bool Actor::getGoodie() const
{
	return m_isGoodie;
}
bool Actor::stillAlive()
{
	return m_alive;
}
void Actor::changeLifeState(bool state)
{
	m_alive = state;
}
StudentWorld* Actor::getWorld()
{
	return m_getWorld;
}
//**********************************************************************//
//						Ships Implementations						   //
//********************************************************************//
Ships::Ships(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld *World, int healthP, bool isAlien, bool isHuman)
	:Actor(imageID, startX, startY, dir, size, depth, World, isAlien, isHuman, false, false), m_healthPoints(healthP) {}
Ships::~Ships() {}
int Ships::getHealthPoints() const
{
	return m_healthPoints;
}
void Ships::lostHealth(int health)
{
	m_healthPoints -= health;
	if (m_healthPoints <= 0)
		changeLifeState(false);
}
void Ships::increaseHP(int health)
{
	m_healthPoints += health;
	if (m_healthPoints > 50)
		m_healthPoints = 50;
}
//**********************************************************************//
//						Nachen Implementations						   //
//********************************************************************//
NachenBlaster::NachenBlaster(StudentWorld *World)
	:Ships(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, World, 50, false, true), m_energyPoints(30), m_torp(0) 
{}
NachenBlaster::~NachenBlaster() 
{}
void NachenBlaster::doSomething()
{
	if (!stillAlive())
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{

		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (getX() > 5)
			{
				moveTo(getX() - 6, getY());
				break;
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getX() + 6 <= VIEW_WIDTH)
			{
				moveTo(getX() + 6, getY());
				break;
			}
			else
			{
				moveTo(VIEW_WIDTH, getY());
				break;
			}
			break;
		case KEY_PRESS_SPACE:
			//cerr << m_energyPoints << endl;
			//add cabbage 12 pix to the right of ship if energy above 5 pts
			if (getEnergyPoints() >= 5)
			{
				//cerr << getEnergyPoints() << endl;
				//create a cabbage to be fired
				Actor* shot = new Cabbage(getX() + 12, getY(), getWorld());
				getWorld()->getActors().push_back(shot); //store cabbage in vector
				setEnergy(getEnergyPoints() - 5); //decrease energy by 5 points
				getWorld()->playSound(SOUND_PLAYER_SHOOT); //play blaster sound
				break;
			}
			else
				break;
		case KEY_PRESS_DOWN:
			if (getY() > 5)
			{
				moveTo(getX(), getY() - 6);
				break;
			}
			break;
		case KEY_PRESS_UP:
			if (getY() + 6 <= VIEW_HEIGHT)
			{
				moveTo(getX(), getY() + 6);
				break;
			}
			break;
		case KEY_PRESS_TAB:
			if (getTorp() > 0)
			{

				FlatulenceTorp* shoot = new FlatulenceTorp(getX() + 12, getY(), 0, getWorld(), false, true);
				getWorld()->getActors().push_back(shoot);
				getWorld()->playSound(SOUND_TORPEDO);
				decTorp();
				break;
			}
			break;
		}
	}
	//Check if it has collided
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getAlien())
			{
				lostHealth((*it)->getDamageValue());
				(*it)->collided();
			}
			if ((*it)->getGoodie())
			{
				(*it)->collided();
			}
		}
		it++;
	}
	if (getEnergyPoints() < 30) //give energy if nachenblaster is not full
	{
		setEnergy(getEnergyPoints() + 1);
	}
}
int NachenBlaster::getEnergyPoints() const
{
	return m_energyPoints;
}
void NachenBlaster::setEnergy(int energyP) //set health points
{
	m_energyPoints = energyP;
}
void NachenBlaster::setTorp()
{
	m_torp++;
}
void NachenBlaster::decTorp()
{
	--m_torp;
}
int NachenBlaster::getTorp()
{
	return m_torp;
}
//**********************************************************************//
//						Alien Implementations						   //
//********************************************************************//
AlienShips::AlienShips(int imageID, double startX, double startY, StudentWorld *World, int healthPoints, double flightSpeed, int flightPlan, int pointValue, int damageValue)
	:Ships(imageID, startX, startY, 0, 1.5, 1.0, World, healthPoints, true, false), m_flightSpeed(flightSpeed), m_flightPlan(flightPlan), m_flightLength(0), 
	m_pointValue(pointValue), m_damageValue(damageValue) 
{}
AlienShips::~AlienShips() {}
int AlienShips::getDamageValue() const
{
	return m_damageValue;
}
//Accessor
int AlienShips::getFlightPlan() const
{
	return m_flightPlan;
}
double AlienShips::getFlightSpeed() const
{
	return m_flightSpeed;
}
int AlienShips::getFlightLength() const
{
	return m_flightLength;
}
//Mutator
void AlienShips::setFlightMember(int f)
{
	m_flightPlan = f;
}
void AlienShips::collided()
{
	changeLifeState(false);
	dropSomething();
	getWorld()->increaseDeadAliens();
	getWorld()->decreaseCurrAliens();
	getWorld()->increaseScore(m_pointValue);
	getWorld()->playSound(SOUND_DEATH);
	Actor* explosion = new Explosion(getX(), getY(), getWorld());
	getWorld()->getActors().push_back(explosion);
}
void AlienShips::setFlightSpeed(double speed)
{
	m_flightSpeed = speed;
}
void AlienShips::setNewFlightPlanLength(int length)
{
	m_flightLength = length;
}
void AlienShips::reduceLength()
{
	m_flightLength--;
}

//**********************************************************************//
//						Small Implementations						   //
//********************************************************************//
Smallgon::Smallgon(double startX, double startY, StudentWorld *World, int healthPoints)
	:AlienShips(IID_SMALLGON, startX, startY, World, healthPoints, 2.0, 0, 250, 5) {}
void Smallgon::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0)
	{
		changeLifeState(false);
		getWorld()->decreaseCurrAliens();
		return;
	}
	//Check if collided
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
				if (stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					collided();
			}
		}
		it++;
	}
	//determine if new flight plan is needed
	if ((getY() >= VIEW_HEIGHT - 1) || getY() <= 0 || getFlightLength() == 0)
	{
		{
			if (getY() >= (VIEW_HEIGHT - 1))//move down then left
			{
				setFlightMember(0);
			}
			else if (getY() <= 0) //move up then left
			{
				setFlightMember(2);
			}
			else if (getFlightLength() == 0)
			{
				setFlightMember(((randInt(0, 2)))); //flight plan length
			}
			setNewFlightPlanLength(randInt(1, 32));

		}

	}
	//chance to fire
	if (getWorld()->getBlaster()->getX() < getX())
	{

		int fireChance = randInt(1, (getWorld()->getLevel()) * 5 + 20);
		if ((getY() <= getWorld()->getBlaster()->getY() + 4) && (getY() >= (getWorld()->getBlaster()->getY() - 4)))
		{
			if (fireChance <= getWorld()->getLevel())
			{
				Actor* turnip = new Turnip(getX(), getY(), getWorld());
				getWorld()->getActors().push_back(turnip);
				getWorld()->playSound(SOUND_ALIEN_SHOOT);
				return;
			}
		}
	}
	//flight algorithm
	switch (getFlightPlan())
	{
	case 0:
		moveTo(getX() - getFlightSpeed(), getY() - getFlightSpeed());//down and left
		reduceLength();
		break;
	case 1:
		moveTo(getX() - getFlightSpeed(), getY()); //straight
		reduceLength();
		break;
	case 2:
		moveTo(getX() - getFlightSpeed(), getY() + getFlightSpeed()); //up and left
		reduceLength();
		break;
	};
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
				if (stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					collided();
			}
		}
		it++;
	}
}
Smallgon::~Smallgon() {}
void Smallgon::dropSomething()
{
	return;
}
//**********************************************************************//
//						Smore Implementations						   //
//********************************************************************//
Smoregon::Smoregon(double startX, double startY, StudentWorld *World, int healthPoints)
	:AlienShips(IID_SMOREGON, startX, startY, World, healthPoints, 2.0, 0, 250, 5)
{}
Smoregon::~Smoregon() {}
void Smoregon::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0)
	{
		changeLifeState(false);
		getWorld()->decreaseCurrAliens();
		return;
	}
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
				if (stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					collided();
			}
			
		}
		it++;
	}
	//determine if new flight plan is needed
	if ((getY() >= VIEW_HEIGHT - 1) || getY() <= 0 || getFlightLength() == 0)
	{
		{
			if (getY() >= (VIEW_HEIGHT - 1))//move down then left
			{
				setFlightMember(0);
			}
			else if (getY() <= 0) //move up then left
			{
				setFlightMember(2);
			}
			else if (getFlightLength() == 0)
			{
				setFlightMember(((randInt(0, 2)))); //flight plan length
			}
			setNewFlightPlanLength(randInt(1, 32));

		}

	}
	//chance to fire
	if (getWorld()->getBlaster()->getX() < getX())
	{

		int fireChance = randInt(1, (getWorld()->getLevel()) * 5 + 20);
		if ((getY() <= getWorld()->getBlaster()->getY() + 4) && (getY() >= (getWorld()->getBlaster()->getY() - 4)))
		{
			if (fireChance <= getWorld()->getLevel())
			{
				Actor* turnip = new Turnip(getX(), getY(), getWorld());
				getWorld()->getActors().push_back(turnip);
				getWorld()->playSound(SOUND_ALIEN_SHOOT);
				return;
			}
			//Chance to ramming speed
			int chanceToRam = randInt(1, 20 / (getWorld()->getLevel()) + 5);
			if (chanceToRam <= getWorld()->getLevel())
			{
				setFlightMember(1);
				setNewFlightPlanLength(VIEW_WIDTH);
				setFlightSpeed(5);
			}
		}
	}

	//flight algorithm
	switch (getFlightPlan())
	{
	case 0:
		moveTo(getX() - getFlightSpeed(), getY() - getFlightSpeed());//down and left
		reduceLength();
		break;
	case 1:
		moveTo(getX() - getFlightSpeed(), getY()); //straight
		reduceLength();
		break;
	case 2:
		moveTo(getX() - getFlightSpeed(), getY() + getFlightSpeed()); //up and left
		reduceLength();
		break;
	};
	//Check collision once again
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
				if (stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					collided();
			}
		}
		it++;
	}
}
void Smoregon::dropSomething()
{
	if (randInt(1, 3) == 1)
	{
		Actor* goodie;
		if (randInt(1, 2) == 1)
			goodie = new Repair(getX(), getY(), getWorld());
		else
			goodie = new FlatulGoodie(getX(), getY(), getWorld());
		getWorld()->getActors().push_back(goodie);
	}
}
//**********************************************************************//
//						Snaggle Implementations						   //
//********************************************************************//
Snagglegon::Snagglegon(double startX, double startY, StudentWorld *World, int healthPoints)
	:AlienShips(IID_SNAGGLEGON, startX, startY, World, healthPoints, 1.75, 1, 1000, 15)
{}
Snagglegon::~Snagglegon() {}
void Snagglegon::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0)
	{
		changeLifeState(false);
		getWorld()->decreaseCurrAliens();
		return;
	}
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	//check if it has collided
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
			}
		}
		it++;
	}
	//Check if it needs a new flight plan
	if ((getY() >= VIEW_HEIGHT - 1) || getY() <= 0)
	{
		{
			if (getY() >= (VIEW_HEIGHT - 1))//move down then left
			{
				setFlightMember(1);
			}
			else if (getY() <= 0) //move up then left
			{
				setFlightMember(2);
			}
		}
	}
	//chance to shoot
	if (getWorld()->getBlaster()->getX() < getX())
	{

		int fireChance = randInt(1, (getWorld()->getLevel()) * 10 + 15);
		if ((getY() <= getWorld()->getBlaster()->getY() + 4) && (getY() >= (getWorld()->getBlaster()->getY() - 4)))
		{
			if (fireChance <= getWorld()->getLevel())
			{
				Actor* torpedo = new FlatulenceTorp(getX(), getY(), 180, getWorld(), true, false);
				getWorld()->getActors().push_back(torpedo);
				getWorld()->playSound(SOUND_TORPEDO);
				return;
			}
		}
	}
	//move it
	switch (getFlightPlan())
	{
	case 1:
		moveTo(getX() - getFlightSpeed(), getY() - getFlightSpeed());//down and left
		break;
	case 2:
		moveTo(getX() - getFlightSpeed(), getY() + getFlightSpeed()); //up and left
		break;
	}
	it = getWorld()->getActors().begin();
	//Check if it has collided again
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				collided();
				(*it)->lostHealth(getDamageValue());
			}
			else if ((*it)->getHuman() && (*it)->getProjectile())
			{
				(*it)->changeLifeState(false);
				lostHealth((*it)->getDamageValue());
				if (stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					collided();
			}
		}
		it++;
	}
}
void Snagglegon::dropSomething()
{
	if (randInt(1, 6) == 1)
	{
		Actor* goodie = new ExtraLife(getX(), getY(), getWorld());
		getWorld()->getActors().push_back(goodie);
	}
}
//**********************************************************************//
//						Stars Implementations						   //
//********************************************************************//
Stars::Stars(double startX, double startY, double size, StudentWorld *World)
	:Actor(IID_STAR, startX, startY, 0, size, 3, World, false, false, false, false) {}
Stars::~Stars() {}
void Stars::doSomething()
{
	//if(stillAlive())
	moveTo(getX() - 1, getY());
	//stillAlive();
	if (getX() <= 0)
		changeLifeState(false);
}
//**********************************************************************//
//						Explosion Implementations					   //
//********************************************************************//
Explosion::Explosion(double startX, double startY, StudentWorld *World)
	:Actor(IID_EXPLOSION, startX, startY, 0, 1.0, 0, World, false, false, false, false), m_timeLeft(4) {}
Explosion::~Explosion() 
{}
void Explosion::doSomething()
{
	if (getLifeTime() == 4)
	{
		decreaseTimeLeft();
		return;
	}
	setSize(getSize() * 1.5); //increase size of explosion
	decreaseTimeLeft(); //decrement life time after 4 ticks object will be removed
	if (getLifeTime() == 0) //object is removed after 4 ticks
	{
		changeLifeState(false);
	}
}
void Explosion::decreaseTimeLeft()
{
	m_timeLeft = m_timeLeft - 1;
}
int Explosion::getLifeTime()
{
	return m_timeLeft;
}


//**********************************************************************//
//						Project Implementations						   //
//********************************************************************//
Projectile::Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld *World, bool isAlien, bool isHuman, int damageValue)
	:Actor(imageID, startX, startY, dir, .5, 1.0, World, isAlien, isHuman, true, false), m_damageValue(damageValue) {}
Projectile::~Projectile() {}
int Projectile::getDamageValue() const
{
	return m_damageValue;
}
void Projectile::collided()
{
	changeLifeState(false);
	getWorld()->playSound(SOUND_BLAST);
}
//**********************************************************************//
//						Cabbage Implementations						   //
//********************************************************************//
Cabbage::Cabbage(double startX, double startY, StudentWorld *World)
	:Projectile(IID_CABBAGE, startX, startY, 0, World, false, true, 2) 
{}
Cabbage::~Cabbage()
{}
void Cabbage::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() >= VIEW_WIDTH)
	{
		changeLifeState(false);
		return;
	}
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getAlien() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					(*it)->collided();
				return;
			}
		}
		it++;
	}
	moveTo(getX() + 8, getY());
	setDirection(getDirection() + 20);
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getAlien() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					(*it)->collided();
				return;
			}
		}
		it++;
	}
}
//**********************************************************************//
//						Turnip Implementations						   //
//********************************************************************//
Turnip::Turnip(double startX, double startY, StudentWorld *World)
	:Projectile(IID_TURNIP, startX, startY, 0, World, true, false, 2) 
{}
Turnip::~Turnip() {}
void Turnip::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0)
	{
		changeLifeState(false);
		return;
	}
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
		it++;
	}
	moveTo(getX() - 6, getY());
	setDirection(20);
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				return;
			}
		}
		it++;
	}
}
//**********************************************************************//
//						FlatTorpedo Implementations					   //
//********************************************************************//
FlatulenceTorp::FlatulenceTorp(double startX, double startY, Direction dir, StudentWorld *World, bool isAlien, bool isHuman)
	: Projectile(IID_TORPEDO, startX, startY, dir, World, isAlien, isHuman, 8) 
{}
FlatulenceTorp::~FlatulenceTorp()
{}
void FlatulenceTorp::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0 || getX() >= VIEW_WIDTH)
	{
		changeLifeState(false);
		return;
	}
	vector<Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile() && getDirection() == 180)
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				return;
			}
			else if ((*it)->getAlien() && !(*it)->getProjectile() && getDirection() == 0)
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					(*it)->collided();
				return;
			}
		}
		it++;
	}
	if (getDirection() == 180)
		moveTo(getX() - 8, getY());
	else
		moveTo(getX() + 8, getY());
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile() && getDirection() == 180)
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				return;
			}
			else if ((*it)->getAlien() && !(*it)->getProjectile() && getDirection() == 0)
			{
				changeLifeState(false);
				(*it)->lostHealth(getDamageValue());
				if ((*it)->stillAlive())
					getWorld()->playSound(SOUND_BLAST);
				else
					(*it)->collided();
				return;
			}
		}
		it++;
	}
}
//**********************************************************************//
//						Goodies Implementations						   //
//********************************************************************//
Goodies::Goodies(int imageID, double startX, double startY, StudentWorld *World)
	:Actor(imageID, startX, startY, 0, .5, 1.0, World, false, false, false, true) {}
Goodies::~Goodies() {}
void Goodies::doSomething()
{
	if (!stillAlive())
		return;
	if (getX() < 0 || getY() < 0)
	{
		changeLifeState(false);
		return;
	}
	vector <Actor*>::iterator it;
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				collided();
				return;
			}
		}
		it++;
	}
	moveTo(getX() - .75, getY() - .75);
	it = getWorld()->getActors().begin();
	while (it != getWorld()->getActors().end())
	{
		if (collisionDetected(*this, **it))
		{
			if ((*it)->getHuman() && !(*it)->getProjectile())
			{
				changeLifeState(false);
				collided();
				return;
			}
		}
		it++;
	}
}
void Goodies::collided()
{
	changeLifeState(false);
	getWorld()->playSound(SOUND_GOODIE);
	getWorld()->increaseScore(100);
	giveBonus();
}
//**********************************************************************//
//						ExtraLife Implementations					   //
//********************************************************************//
ExtraLife::ExtraLife(double startX, double startY, StudentWorld *World)
	:Goodies(IID_LIFE_GOODIE, startX, startY, World) {}
ExtraLife::~ExtraLife() {}
void ExtraLife::giveBonus()
{
	getWorld()->incLives();
}
//**********************************************************************//
//						Repair Implementations						   //
//********************************************************************//
Repair::Repair(double startX, double startY, StudentWorld *World)
	:Goodies(IID_REPAIR_GOODIE, startX, startY, World) {}
Repair::~Repair() {}
void Repair::giveBonus()
{
	getWorld()->getBlaster()->increaseHP(10);
}
//**********************************************************************//
//						FlatGoodie Implementations					   //
//********************************************************************//
FlatulGoodie::FlatulGoodie(double startX, double startY, StudentWorld *World)
	:Goodies(IID_TORPEDO_GOODIE, startX, startY, World) {}
FlatulGoodie::~FlatulGoodie() {}
void FlatulGoodie::giveBonus()
{
	getWorld()->getBlaster()->setTorp();
}