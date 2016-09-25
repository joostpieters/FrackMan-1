#include "StudentWorld.h"
#include "GameConstants.h"
#include<stack>
#include <string>
#include <cstdlib>
#include<cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir),health(100),water(5),goldnum(0),sonar(1),Tickcount(0)
{
}
StudentWorld::~StudentWorld()
{
    for(int i=0; i<m_actor.size(); i++)
        delete m_actor.at(i);
    for(int i=0;i<VIEW_WIDTH;i++)
        for(int j=0;j<VIEW_HEIGHT;j++)
            delete map[i][j];
    m_actor.clear();
}

int StudentWorld::init()
{
    health=100;
     int m_level=getLevel();
    FrackMan* player = new FrackMan(this); //create the player and put it into the vector
    m_actor.push_back(player);
    
   
   
    for (int x = 0; x < 30; x++)
        for (int y = 0; y < 60; y++)
            map[x][y] = new Dirt(this, x, y);
    for (int x = 34; x < 64; x++)
        for (int y = 0; y < 60; y++)
            map[x][y] = new Dirt(this, x, y);
    for (int x = 30; x < 34; x++)
        for (int y = 0; y < 4; y++)
            map[x][y] = new Dirt(this, x, y);
    for (int x = 30; x < 34; x++)
        for (int y = 4; y < 60; y++)
            map[x][y] = new GraphObject(0,x,y);///virtual class will not be constructed
    for (int x = 0; x < 64; x++)
        for (int y = 60; y < 64; y++)
            map[x][y] = new GraphObject(0,x,y);
    //initialize the number of content
    int B = min(m_level / 2 + 2, 6);//boulder
    for (int i=0;i<B;i++)
    {
        int x=rand()% 61; int y=rand()%37+20;
        
        while(generatelocation(x, y)==false||(x<34&&x>26))
        {
            x=rand()% 61 ;
            y=rand()%37+20;
        }
        
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
                delete map[x+i][y+j];
                map[x+i][y+j]=nullptr;
            }
        Boulder*rock=new Boulder(this,x,y);
        m_actor.push_back(rock);
    }
        
    int G = max(5-m_level / 2, 2);//gold
    for (int i=0;i<G;i++)
    {
        int x=rand()% 61; int y=rand()%57;
        
        while(generatelocation(x, y)==false||(x<34&&x>26))
        {
            x=rand()% 61 ;
            y=rand()%57;
        }
        
        GoldNugget*gold=new GoldNugget(this,x,y,true);
        m_actor.push_back(gold);
    }


    //displace the contant randomly within the map
    int L = min(2 + m_level, 20);
    for (int i=0;i<L;i++)
    {
        int x=rand()% 61; int y=rand()%57;
        
        while(generatelocation(x, y)==false||(x<34&&x>26))
        {
            x=rand()% 61 ;
            y=rand()%57;
        }
        
        Barrel*oil=new Barrel(this,x,y);
        m_actor.push_back(oil);
    }
///initialize a protestor at the beginning
    int probabilityOfHardcore = fmin(90, getLevel() * 10 + 30);
    int x=rand()% probabilityOfHardcore;
    if(x==1)
    {
        hardcoreProtestor*enemy=new hardcoreProtestor(this, IID_HARD_CORE_PROTESTER, 60, 60);
        m_actor.push_back(enemy);
    }
    else{
        Protestor*enemy=new Protestor(this,IID_PROTESTER,60,60);
        m_actor.push_back(enemy);
    }

    ////cout the string
    oil_left=L;
    string text = "Scr: " + std::to_string(getScore()) +
    "  Lvl: " + std::to_string(getLevel()) +
    "  Lives:" + std::to_string(getLives()) +
    "  Hlth: " + std::to_string(health) +
    "%  Wtr : " + std::to_string(water) +
    "  Gld : " + std::to_string(goldnum) +
    "  Sonar : " + std::to_string(sonar) +
    "  Oil Left : " + std::to_string(oil_left);
    setGameStatText(text);
    return 0;
}

int StudentWorld::move()
{
    string score = std::to_string((unsigned long)getScore());
    while (score.length() < 8)
        score.insert(0, "0");
    string text = "Scr: " + score + "  Lvl: " +
    std::to_string(getLevel()) + "  Lives:" + std::to_string(getLives()) +
    "  Hlth: " + std::to_string(health) + "%  Water: " +
    std::to_string(water) + "  Gld: " + std::to_string(goldnum) +
    "  Sonar: " + std::to_string(sonar) + "  Oil Left: " + std::to_string(oil_left);
    setGameStatText(text);
    
    int G = getLevel() * 25 + 300;
    int x=rand()% G;
    if(x==0)
    {
        int y=rand()% 5;
        if(y==0)
        {
            Sonar*radar=new Sonar(this,0,60);
            m_actor.push_back(radar);
        }
        if(y>0)
        {
            int x_w=rand()%61; int y_w= rand()%61;
            while(Dirtoverlap(x_w, y_w))
            {
                x_w=rand()%61;y_w= rand()%61;
            }
            Water*amo=new Water(this,x_w,y_w);
            m_actor.push_back(amo);
            
        }//add water
    }
    int T = fmax(25, 200 -getLevel());
    int P = fmin(15, 2 + getLevel() * 1.5);//protestor capacity
    int probabilityOfHardcore = fmin(90, getLevel() * 10 + 30);
    Tickcount++;
    if(countProtestor()<P)
    {
        if(Tickcount>=T)
        {
            int x=rand()% probabilityOfHardcore;
            if(x==0)
            {
                hardcoreProtestor*enemy=new hardcoreProtestor(this, IID_HARD_CORE_PROTESTER, 60, 60);
                m_actor.push_back(enemy);
            }
            else{
            Protestor*enemy=new Protestor(this,IID_PROTESTER,60,60);
            m_actor.push_back(enemy);
            }
            Tickcount=0;
        }
       
    }
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor.at(i)->isAlive())
            m_actor.at(i)->doSomething();
  
        else
        {
         if(m_actor.at(i)->getID()==IID_PLAYER)
         {
             decLives();
             return GWSTATUS_PLAYER_DIED;
         }
         else
             delete m_actor.at(i);
            vector<Actor*>::iterator iter=m_actor.begin(); //create an iterator for the pointer index to be erased
            for(int j=0; j<i; j++)
                iter++;
            m_actor.erase(iter);
            i--;
        }
    }
    if(oil_left==0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
    for(int i=0; i<m_actor.size(); i++)
        delete m_actor.at(i);
    for(int i=0;i<VIEW_HEIGHT;i++)
        for(int j=0;j<VIEW_WIDTH;j++)
            delete map[i][j];
    m_actor.clear();
}


///////////////implementatioins///////////////////////////
void StudentWorld::killPlayer(int m_health)
{
    health-=m_health;
    if(health<=0)
    {
        for(int i=0; i<m_actor.size(); i++)
        {
            if( m_actor.at(i)->getID()==IID_PLAYER) //if player is at x,y
                m_actor.at(i)->setDead(); //set the player to dead
        }
    }
}
bool StudentWorld::generatelocation(int x, int y)
{
    for(int i=0; i<m_actor.size();i++)
    {
        int x_loc=m_actor.at(i)->getX();
        int y_loc=m_actor.at(i)->getY();
        int result = pow((y_loc-y),2)+pow((x_loc-x),2);
        if(result<=36)
            return false;

    }
    return true;
}
bool StudentWorld::encounterobject(int ID,int x, int y,int radius)
{
    bool encounter=false;
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==ID)
        {
            int x_loc=m_actor.at(i)->getX();
            int y_loc=m_actor.at(i)->getY();
            
                    int result=pow((x_loc-x),2)+pow((y_loc-y),2);
                    if(result<(radius*radius))
                    {
                        encounter = true;
                        if(id==IID_WATER_SPURT)
                            m_actor.at(i)->setDead();
                        break;
                    }
            
        }
    }
    return encounter;
}
bool StudentWorld::encounterprotestor(int x,int y,int radius)
{
    bool encounter=false;
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==IID_HARD_CORE_PROTESTER||id==IID_PROTESTER)
        {
            int x_loc=m_actor.at(i)->getX();
            int y_loc=m_actor.at(i)->getY();
            
                    int result=pow((x_loc-x),2)+pow((y_loc-y),2);
                    if(result<(radius*radius))
                    {
                        encounter = true;
                        break;
                    }
        }
    }
    return encounter;
}
void StudentWorld::pushitem(Actor *item)
{
    m_actor.push_back(item);
}
////water implementation////
bool StudentWorld::fireable(int x, int y, Actor::Direction dir)
{
    if(dir==Actor::up)
        for(int i=0;i<4;i++)
            if(Boulderthere(x, y+i)||Dirtoverlap(x, y+i))
                return false;
    if(dir==Actor::down)
        for(int i=0;i<4;i++)
            if(Boulderthere(x, y-i)||Dirtoverlap(x, y-i))
                return false;
    if(dir==Actor::left)
        for(int i=0;i<4;i++)
            if(Boulderthere(x-i, y)||Dirtoverlap(x-i, y))
                return false;
    if(dir==Actor::right)
        for(int i=0;i<4;i++)
            if(Boulderthere(x+i, y)||Dirtoverlap(x+i, y))
                return false;
    return true;
}
////////////Dirt implementation
bool StudentWorld::Dirtoverlap(int x,int y)
{
    if(x<=60&&y<60)
    {
        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 3; j++)
            {
                if (map[x+i][y+j] != nullptr && map[x+i][y+j]->getID() == IID_DIRT)
                {
                return true;
                }
            }
        }
    }
    return false;
}

void StudentWorld::removeDirt(int x, int y,int dir)
{
    if(dir==Actor::up)
        for (int j = 0; j <= 3; j++)
            if(map[x+j][y+3]!=nullptr&&map[x+j][y+3]->getID()==IID_DIRT) {
                delete map[x+j][y+3];
                map[x+j][y+3] = nullptr;
            }
    if(dir==Actor::down)
        for (int j = 0; j <= 3; j++)
            if(map[x+j][y]!=nullptr&&map[x+j][y]->getID()==IID_DIRT)
            {
                delete map[x+j][y];
                map[x+j][y] = nullptr;
            }

    if(dir==Actor::left)
        for (int j = 0; j <= 3; j++)
            if(map[x][y+j]!=nullptr&&map[x][y+j]->getID()==IID_DIRT)
            {
                delete map[x][y+j];
                map[x][y+j] = nullptr;
            }
    if(dir==Actor::right)
        for (int j = 0; j <= 3; j++)
            if(map[x+3][y+j]!=nullptr&&map[x+3][y+j]->getID()==IID_DIRT)
            {
                delete map[x+3][y+j];
                map[x+3][y+j] = nullptr;
            }
}
//////////////////Boulder implementation//////////////
bool StudentWorld::DirtunderBoulder(int x,int y)
{
    for (int i=0; i<4;i++)
    {
        if(map[x+i][y-1]!=nullptr&&map[x+i][y-1]->getID()==IID_DIRT)
            return true;
    }
    return false;
}
bool StudentWorld::movedownward(int x, int y)//problematic
{
    if(y<=0||Dirtoverlap(x, y)||Boulderthere(x,y-4)==true)
        return false;
    return true;
}



bool StudentWorld::Boulderthere(int x, int y)
{
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        
        if(id==IID_BOULDER) //for each boulder in the world
        {
            int tempx=m_actor.at(i)->getX(), tempy=m_actor.at(i)->getY();
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                {
                    if(x==tempx+i && y==tempy+j) //if there is a boulder at that spot, return true
                        return true;
                }
        }
    }
    return false;
}
///////Gold implementation/////////
void StudentWorld::setgold(char method, int num)
{
    if(method=='+')
        goldnum+=num;
    if(method=='-')
        goldnum-=num;
}

//sonar implementation//
void StudentWorld::detect()
{
    for(int i=0; i<m_actor.size(); i++)
    {
      int tempx=m_actor.at(i)->getX(), tempy=m_actor.at(i)->getY();
        if(encounterobject(IID_PLAYER,tempx,tempy,12))
            m_actor.at(i)->setVisible(true);
    }

}
//Protestor implementation//


void StudentWorld::intialMaze(int maze[][64]) {
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            if (Dirtoverlap(i, j)||Boulderthere(i, j)||i>60||j>60||i<0||j<0)
                maze[i][j] = -1;
            else
                maze[i][j] = 0;
        }
    }
}
void StudentWorld::explore(int maze[][64],std::queue<std::pair<int,int>> &toDo,int x,int y,int level)
{
    if (maze[x][y] == 0)
    {
        toDo.push(make_pair(x,y));
        maze[x][y] = level;  // anything non- -1 will do

    }
}



Actor::Direction StudentWorld::getplayerdirection(int x, int y)
{
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==IID_PLAYER)
        {
            int x_loc=m_actor.at(i)->getX();
            int y_loc=m_actor.at(i)->getY();
            if(x_loc==x&&y_loc>y)
                return Actor::up;
            else if(x_loc==x&&y_loc<y)
                return Actor::down;
            else if(x_loc>x&&y_loc==y)
                return Actor::right;
            else if(x_loc<x&&y_loc==y)
                return Actor::left;
            
        }
    }
    return Actor::none;
    
}
bool StudentWorld::canmove(int x, int y)
{
    int x_loc = 0; int y_loc=0;
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==IID_PLAYER)
        {
            x_loc=m_actor.at(i)->getX();
            y_loc=m_actor.at(i)->getY();
        }
    }
    Actor::Direction dir=getplayerdirection(x, y);
    switch(dir)
    {
        case Actor::right :
            for(int i=x;i<x_loc;i++)
                if(Dirtoverlap(i,y)||Boulderthere(i, y)||x<0||x_loc>60)
                    return false;
        case Actor::left :
            for(int i=x_loc;i<x;i++)
                if(Dirtoverlap(i,y)||Boulderthere(i, y)||x_loc<0||x>60)
                    return false;
        case Actor::up :
            for(int i=y;i<y_loc;i++)
                if(Dirtoverlap(x,i)||Boulderthere(x, i)||y<0||y_loc>60)
                    return false;
        case Actor::down :
            for(int i=y_loc;i<y;i++)
                if(Dirtoverlap(x,i)||Boulderthere(x, i)||y_loc<0||y>60)
                    return false;
        default:
            return true;
            
    
            
    }
}
Actor::Direction StudentWorld::randDirection()
{
    int x=rand()%4;
    if(x==0)
        return Actor::up;
    if(x==1)
        return Actor::down;
    if(x==2)
        return Actor::right;
    if(x==3)
        return Actor::left;
    return Actor::none;
}
bool StudentWorld::movable(int x,int y,Actor::Direction dir)
{
    if(dir==Actor::up)
        if(!Dirtoverlap(x, y+1)&&!Boulderthere(x, y+1)&&y+1<=60)
            return true;
    if(dir==Actor::down)
        if(!Dirtoverlap(x, y-1)&&!Boulderthere(x, y-1)&&y-1>=0)
            return true;
    if(dir==Actor::right)
        if(!Dirtoverlap(x+1, y)&&!Boulderthere(x+1, y)&&x+1<=60)
            return true;
    if(dir==Actor::left)
        if(!Dirtoverlap(x-1, y)&&!Boulderthere(x-1, y)&&x-1>=0)
            return true;
    return false;
}
int StudentWorld::countProtestor()
{
    int count=0;
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==IID_PROTESTER||id==IID_HARD_CORE_PROTESTER)
        {
            count++;
        }
    }
    return count;
}
bool StudentWorld::intersection(int x, int y,Actor::Direction dir)
{
    if(dir==Actor::up||dir==Actor::down)
        if(movable(x,y,Actor::left)||movable(x,y,Actor::right))
           return true;
    if(dir==Actor::left||dir==Actor::right)
        if(movable(x,y,Actor::up)||movable(x,y,Actor::down))
         return true;
    return false;
}
bool StudentWorld::visualonPlayer(int x, int y)
{
    if(getplayerdirection(x, y)==Actor::none)
        return false;
    else
    {
        if(!canmove(x, y))
            return false;
        else
            return true;
    }
}
Actor::Direction StudentWorld::pickperpendir(int x, int y,Actor::Direction dir)
{
    if(dir==Actor::right||dir==Actor::left)
    {
        if(!movable(x, y,Actor::up ))
            return Actor::down;
        if(!movable(x, y,Actor::down))
           return Actor::up;
    
        else
        {
            int x=rand()%2;
            if(x==1) return Actor::up;
            else return Actor::down;
        }
    }
    else
    {
        if(!movable(x, y,Actor::left ))
            return Actor::right;
        if(!movable(x, y,Actor::right))
            return Actor::left;
        
        else
        {
            int x=rand()%2;
            if(x==1) return Actor::right;
            else return Actor::left;
        }
   
    }
}
/////////////hardcore protestor////////////
//
int StudentWorld::getlev(int (*maze)[64],int x,int y)
{
    return maze[x][y];
}
int StudentWorld::exitmaze(int x, int y,int endx,int endy,std::queue<std::pair<int,int>> some,int &getx,int &gety)
{
    int level=1;
    int maze[64][64];
    std::stack<int> m_level;
    
    intialMaze(maze);
    some.push(std::make_pair(endx,endy));
    maze[endx][endy]=1;
    m_level.push(-1);
    
    while ( ! some.empty() )
    {
        
        int x_loc = some.front().first;
        int y_loc=some.front().second;
        // std::cerr<<"current x:"<<x_loc<<"current y:"<<y_loc<<std::endl;
        int prevlevel=getlev(maze,x_loc,y_loc);
        
        some.pop();
        
        
        if ((x == x_loc+1  &&  y_loc == y)||(x==x_loc-1&&y_loc==y)||(x==x_loc&&y_loc==y-1)||(x==x_loc&&y_loc==y+1))
        {
            //std::cerr<<"current x:"<<x<<"current y:"<<y<<std::endl;
            getx=x_loc;  gety=y_loc;
           // std::cerr<<"level"<<maze[x_loc][y_loc]<<std::endl;
            return maze[x_loc][y_loc];
            
        }
        if(m_level.top()!=maze[x_loc][y_loc])
        {
            level++;
            m_level.push(prevlevel);
            
        }
        
        explore(maze, some, x_loc-1, y_loc,level);  // north
        explore(maze, some, x_loc, y_loc+1,level);  // east
        explore(maze, some, x_loc+1, y_loc,level);  // south
        explore(maze, some, x_loc, y_loc-1,level);  // west
    }
    return 0;
    
}

void StudentWorld::getPlayerloc(int &x,int &y)
{
    for(int i=0; i<m_actor.size(); i++)
    {
        int id = m_actor.at(i)->getID();
        if(id==IID_PLAYER)
        {
            int x_loc=m_actor.at(i)->getX();
            int y_loc=m_actor.at(i)->getY();
            x=x_loc;y=y_loc;
        }
    }
}