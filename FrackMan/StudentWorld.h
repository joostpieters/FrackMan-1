#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include"Actor.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include"GraphObject.h"
#include <string>
#include<queue>
#include <vector>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    ////implementations//
    
    void killPlayer(int m_health);
    bool generatelocation(int x,int y);
    bool encounterobject(int ID,int x, int y,int radius);
    bool encounterprotestor(int x,int y,int radius);
    void pushitem(Actor*item);
    ///water implementation///
    int getwater(){return water;}
    void changewater(char method){if(method=='+')water+=5;if(method=='-')water--;}
    bool fireable(int x,int y,Actor::Direction dir);
    //Dirt implementation//
    bool Dirtoverlap(int x,int y);
    void removeDirt(int x,int y,int dir);
    //Boulder implementations//
    bool DirtunderBoulder(int x,int y);
    bool movedownward(int x,int y);
    bool Boulderthere(int x,int y);
    //Gold implementation//
    void setgold(char method,int num);
    int getgold(){return goldnum;}
//    void bribeProtestor(int x , int y,int hitpoint);
    //oil implementation
    void addoil(){oil_left--;}
    //sonar implementation
    void addsonar(char method){if(method=='+')sonar++;if(method=='-')sonar--;}
    int getsonar(){return sonar;}
    void detect();
    //protester implementation//
    void intialMaze(int maze[][64]);
    void explore(int maze[][64],std::queue<std::pair<int,int>> &toDo,int x,int y,int level);

    Actor::Direction getplayerdirection(int x,int y);
    bool canmove(int x,int y);
    bool movable(int x,int y,Actor::Direction dir);
    void moveProtestor(int x,int y,Actor::Direction dir);
    Actor::Direction randDirection();
    int countProtestor();
    bool intersection(int x,int y,Actor::Direction dir);
    bool visualonPlayer(int x,int y);
    Actor::Direction pickperpendir(int x,int y,Actor::Direction dir);
    void getPlayerloc(int &x,int &y);
    //////hardcore protestor//////////
    int getlev(int (*maze)[64],int x,int y);
    int exitmaze(int x, int y,int endx,int endy,std::queue<std::pair<int,int>> some,int &getx,int &gety);

        private:
    std::vector<Actor*> m_actor;
    GraphObject *map[VIEW_WIDTH][VIEW_HEIGHT];
    unsigned int health;
    unsigned int water;
    unsigned int goldnum;
    unsigned int sonar;
    unsigned int oil_left;
    int Tickcount;
};

#endif // STUDENTWORLD_H_
