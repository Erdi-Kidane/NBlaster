#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

//Actor class
class Actor :public GraphObject
{
public:
	Actor(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld *World, bool isAlien, bool isHuman,
		bool isProjectile, bool isGoodie);
	virtual ~Actor();
	virtual void doSomething() = 0;
	virtual int getDamageValue() const;
	virtual void collided();
	virtual void lostHealth(int damage);
	bool getAlien() const;
	bool getHuman() const;
	bool getProjectile() const;
	bool getGoodie() const;
	//tests to see if a particular actor is alive or not
	bool stillAlive();
	//all actors have a life or death status this method allows us to change that
	void changeLifeState(bool state);
	//access to student world for actors
	StudentWorld* getWorld();
private:
	StudentWorld* m_getWorld;
	bool m_alive, m_isAlien, m_isHuman, m_isProjectile, m_isGoodie;
};
//Stars class
class Stars :public Actor
{
public:
	Stars(double startX, double startY, double size, StudentWorld *World);
	virtual ~Stars();
	//do Something for stars
	virtual	void doSomething();
};
class Ships :public Actor
{
public:
	Ships(int imageID, double startX, double startY, Direction dir, double size, unsigned int depth, StudentWorld *World, int healthP, bool isAlien, bool isHuman);
	virtual ~Ships();
	//get health points
	int getHealthPoints() const;
	//set health points
	void lostHealth(int health); 
	void increaseHP(int health);
	virtual void doSomething() = 0;
private:
	int m_healthPoints;
};
//NachenBlaster class
class NachenBlaster :public Ships
{
public:
	NachenBlaster(StudentWorld *World);
	virtual ~NachenBlaster();
	virtual void doSomething();
	//get energy points
	int getEnergyPoints() const;
	//set health points
	void setEnergy(int energyP);
	void setTorp();
	void decTorp();
	int getTorp();
private:
	int m_torp;
	int m_energyPoints;
};
class Projectile :public Actor
{
public:
	Projectile(int imageID, double startX, double startY, Direction dir, StudentWorld *World, bool isAlien, bool isHuman, int damageValue);
	virtual ~Projectile();
	virtual int getDamageValue() const;
	virtual void collided();
private:
	int m_damageValue;
};
//Cabbage class
class Cabbage :public Projectile
{
public:
	Cabbage(double startX, double startY, StudentWorld *World);
	virtual void doSomething();
	virtual ~Cabbage();
};
class Turnip :public Projectile
{
public:
	Turnip(double startX, double startY, StudentWorld *World);
	virtual ~Turnip();
	virtual void doSomething();
};
class FlatulenceTorp :public Projectile
{
public:
	FlatulenceTorp(double startX, double startY, Direction dir, StudentWorld *World, bool isAlien, bool isHuman);
	virtual ~FlatulenceTorp();
	virtual void doSomething();
};
class Explosion :public Actor
{
public:
	Explosion(double startX, double startY, StudentWorld *World);
	virtual void doSomething();
	void decreaseTimeLeft();
	int getLifeTime();
	virtual ~Explosion();

private:
	int m_timeLeft;
};
//alienShip class can fire turnups and has flight plan,
class AlienShips :public Ships
{
public:
	AlienShips(int imageID, double startX, double startY, StudentWorld *World, int healthPoints, double flightSpeed, int flightPlan, int pointValue, int damageValue);
	virtual int getDamageValue() const;
	//Accessor
	int getFlightPlan() const;
	double getFlightSpeed() const;
	int getFlightLength() const;
	//Mutator
	void setFlightMember(int f);
	virtual void collided();
	void setFlightSpeed(double speed);
	void setNewFlightPlanLength(int length);
	void reduceLength();
	virtual void dropSomething() = 0;
	virtual ~AlienShips();

private:
	int m_flightLength;
	double m_flightSpeed;
	int m_flightPlan;
	int m_pointValue;
	int m_damageValue;
};
//Smallgon is a type of Alienship
class Smallgon :public AlienShips
{
public:
	Smallgon(double startX, double startY, StudentWorld *World, int healthPoints);
	virtual void doSomething();
	virtual ~Smallgon();
	virtual void dropSomething();
};
//Smoregon is a type of AlienShip
class Smoregon :public AlienShips
{
public:
	Smoregon(double startX, double startY, StudentWorld *World, int healthPoints);
	virtual void doSomething();
	virtual ~Smoregon();
	virtual void dropSomething();
};
//Snagglegon is a type of alienship
class Snagglegon : public AlienShips
{
public:
	Snagglegon(double startX, double startY, StudentWorld *World, int healthPoints);
	virtual ~Snagglegon();
	virtual void doSomething();
	virtual void dropSomething();
};
//goodies are things that the nachenBlaster can pick up.  They are dropped by snagglegons and smoregons
class Goodies :public Actor
{
public:
	Goodies(int imageID, double startX, double startY, StudentWorld *World);
	virtual ~Goodies();
	virtual void doSomething();
	virtual void collided();
	virtual void giveBonus() = 0;
};
class ExtraLife :public Goodies
{
public:
	ExtraLife(double startX, double startY, StudentWorld *World);
	virtual ~ExtraLife();
	virtual void giveBonus();
};
class Repair :public Goodies
{
public:
	Repair(double startX, double startY, StudentWorld *World);
	virtual ~Repair();
	virtual void giveBonus();
};
class FlatulGoodie :public Goodies
{
public:
	FlatulGoodie(double startX, double startY, StudentWorld *World);
	virtual ~FlatulGoodie();
	virtual void giveBonus();
};
//Extra Functions

bool collisionDetected(const Actor &A, const Actor &B); //polymorphism detect if two objects have collided
double euclidian_dist(double x1, double y1, double x2, double y2);
#endif 
// ACTOR_H_
