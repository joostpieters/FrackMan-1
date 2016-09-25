#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include<queue>
class StudentWorld;

class Actor:public GraphObject
{
public:
    Actor(StudentWorld*world,int imageID, int startX, int startY, Direction dir, double size , unsigned int depth);
    virtual ~Actor();
    virtual void doSomething()=0;
    bool isAlive();
    void setDead();
    StudentWorld*getworld() const;
    private:
    StudentWorld*myworld;
    bool m_alive;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Dirt : public Actor {
public:
    Dirt(StudentWorld* world, int x, int y);
    virtual void doSomething();
    virtual ~Dirt();
private:
    
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class FrackMan:public Actor
{
public:
    FrackMan(StudentWorld*world);
    virtual ~FrackMan();
    void moveplayer(int dir);
    virtual void doSomething();
private:
    int hitpoint;
    int water;
    int sonarcharge;
    int gold;
    
};

class Boulder:public Actor
{
public:
    Boulder(StudentWorld*world,int x,int y);
    virtual void doSomething();
    virtual ~Boulder();
private:
    int m_tick;
};
///GoldNugget
class GoldNugget:public Actor
{
public:
    GoldNugget(StudentWorld*world,int x, int y,bool state);
    virtual void doSomething();
    virtual ~GoldNugget();
private:
    bool permanent;
    int m_tick;
};
//squirt
class squirt:public Actor
{
public:
    squirt(StudentWorld*world,int x,int y,Direction dir);
    virtual ~squirt();
    virtual void doSomething();
private:
    int m_travel;
};
//Barrels of oil
class Barrel:public Actor
{
public:
    Barrel(StudentWorld*world,int x,int y);
    virtual void doSomething();
    virtual ~Barrel();
private:
    bool permanent;
    
};
//sonar class
class Sonar:public Actor
{
public:
    Sonar(StudentWorld*world,int x,int y);
    virtual ~Sonar();
    virtual void doSomething();
private:
    int m_ticks;
};
class Water:public Actor
{
public:
    Water(StudentWorld*world,int x,int y);
    virtual ~Water();
    virtual void doSomething();
private:
    int m_ticks;
};
//class protestor///
class Protestor:public Actor
{
public:
    Protestor(StudentWorld*world,int ID,int x,int y);
    virtual ~Protestor();
    virtual void doSomething();
    virtual void annoyprotestor();
    void changeDirection(int curx,int cury,int destx,int desty);
    void moveProtestor(int x,int y,Direction dir,int &squaretomove);


protected:
    std::queue<std::pair<int,int>> path;
    bool chase;
    int SquareToMove;
    int hitpoint;
    bool leavefield;
    int shoutcount;
    bool shout;
    int m_tick;
    int turn;
    int tickToPause;
    bool dead;
};
//class harcoreProtestor

class hardcoreProtestor:public Protestor
{
public:
    hardcoreProtestor(StudentWorld*world,int ID,int x,int y);
    virtual ~hardcoreProtestor();
    virtual void doSomething();
    virtual void annoyprotestor();


};
#endif // ACTOR_H_