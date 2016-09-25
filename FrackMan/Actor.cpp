#include "Actor.h"
#include "StudentWorld.h"
#include"GameConstants.h"
#include<stack>
#include <cmath>
#include<cstdlib>
///////////////Actor class//////////////////////
Actor::Actor(StudentWorld*world,int imageID, int startX, int startY, Direction dir, double size , unsigned int depth)
:GraphObject(imageID,startX,startY,dir,size,depth),myworld(world),
m_alive(true){setVisible(true);}

bool Actor::isAlive()
{
    return m_alive;
}

void Actor::setDead()
{
   m_alive=false;
}

StudentWorld*Actor::getworld() const
{
    return myworld;
}

Actor::~Actor()
{
    setVisible(false);
}
/////////////////Dirt class////////////
Dirt::Dirt(StudentWorld* world, int x, int y)
:Actor(world,IID_DIRT, x, y, right, 0.25, 3){
    setVisible(true);
}

void Dirt::doSomething() {
    return;
}

Dirt::~Dirt() {}
////////////////////FrackMan class//////////////////////////////////////
FrackMan::FrackMan(StudentWorld*myworld)
:Actor(myworld, IID_PLAYER, 30, 60, right, 1.0, 0)
{
    hitpoint = 10;
    water = 5;
    sonarcharge = 1;
    gold = 0;
}
void FrackMan::moveplayer(int dir)
{
    bool canmove=true;
    int x_pos;int y_pos;
    x_pos=getX();
    y_pos=getY();
    if(dir==KEY_PRESS_LEFT)
    {
        if(getDirection()==left&&x_pos>0)
        {
            for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
            {
            if(getworld()->Boulderthere(x_pos+i-1,y_pos+j))
                canmove=false;
            }
            if(canmove==true)
                moveTo(x_pos-1, y_pos);
        }
        else if(getDirection()!=left)
                setDirection(left);
    }
    if(dir==KEY_PRESS_RIGHT)
    {
        if(getDirection()==right&&x_pos<60)
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                {
                    if(getworld()->Boulderthere(x_pos+i+1,y_pos+j))
                        canmove=false;
                }
            if(canmove==true)
                moveTo(x_pos+1, y_pos);

        }
        else if(getDirection()!=right)
            setDirection(right);
    }
    if(dir==KEY_PRESS_UP)
    {
        if(getDirection()==up&&y_pos<60)
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                {
                    if(getworld()->Boulderthere(x_pos+i,y_pos+j+1))
                        canmove=false;
                }
            if(canmove==true)
                moveTo(x_pos, y_pos+1);

        }
        else if(getDirection()!=up)
            setDirection(up);
    }
    if(dir==KEY_PRESS_DOWN)
    {
        if(getDirection()==down&&y_pos>0)
        {
            for(int i=0;i<4;i++)
                for(int j=0;j<4;j++)
                {
                    if(getworld()->Boulderthere(x_pos+i,y_pos+j-1))
                        canmove=false;
                }
            if(canmove==true)
                moveTo(x_pos, y_pos-1);
            
        }
        else if(getDirection()!=down)
            setDirection(down);
    }



}
void FrackMan::doSomething()
{
    Direction dirtdir=getDirection();
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world= getworld();
    if(!isAlive())
        return;
    if(m_world->Dirtoverlap(x_loc,y_loc))
       {
           m_world->removeDirt(x_loc,y_loc,dirtdir);
           m_world->playSound(SOUND_DIG);
       }
    int dir;
    if(m_world->getKey(dir))
    {
        switch(dir)
        {
        case KEY_PRESS_SPACE:
                //... add a Squirt in front of the player...;
                if(m_world->getwater()>0)
                {
                    if(!m_world->fireable(x_loc,y_loc,dirtdir))
                    {
                        m_world->changewater('-');
                        m_world->playSound(SOUND_PLAYER_SQUIRT);
                    }
                    else
                    {

                    m_world->changewater('-');
                    squirt*bullet=new squirt(m_world,x_loc,y_loc,dirtdir);
                    m_world->pushitem(bullet);
                    }
                }
            break;
        case 'Z':
        case 'z':
                if(m_world->getsonar()>0)
                {
                    m_world->addsonar('-');
                    m_world->detect();
                }
                break;
        case KEY_PRESS_ESCAPE:
                setDead();
                break;
        case KEY_PRESS_TAB:
                if(m_world->getgold()>0)
                {
                    GoldNugget*bribe=new GoldNugget(m_world,x_loc,y_loc,false);
                    m_world->pushitem(bribe);
                    m_world->setgold('-', 1);
                }
            default:
                moveplayer(dir);
                break;
        }
    }
}
FrackMan::~FrackMan()
{}
//////////////////Boulder//////////////////////
Boulder::Boulder(StudentWorld*world,int x,int y)
:Actor(world, IID_BOULDER,x,y,down,1,1),m_tick(30)
{}
Boulder::~Boulder()
{}
void Boulder::doSomething()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world= getworld();
    
    if(!isAlive())
        return;
    if(!m_world->DirtunderBoulder(x_loc,y_loc) && m_tick > 0)
    {
        m_tick--;//wait for 30 ticks
        return;
    }
    if(m_tick==0)
    {
            
        m_world->playSound(SOUND_FALLING_ROCK);//play sound
        if(m_world->movedownward(x_loc, y_loc)==true)
        {
            y_loc--;
            moveTo(x_loc,y_loc);
            if(m_world->encounterobject(IID_PLAYER,x_loc,y_loc,3))
                m_world->killPlayer(100);
            
            //encounter protestor will be dealled in protestor class
        }
            
            
    }
    if (!m_world->movedownward(x_loc, y_loc))
        setDead();
    
    
}
///////////////////Gold Nugget/////////////
GoldNugget::GoldNugget(StudentWorld*world,int x, int y,bool pickable)
:Actor(world,IID_GOLD,x,y,right,1,2),permanent(pickable),m_tick(100)
{setVisible(!permanent);}

GoldNugget::~GoldNugget()
{}

void GoldNugget::doSomething()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world= getworld();
    if(!isAlive())
        return;
    if(!isVisible()&&m_world->encounterobject(IID_PLAYER,x_loc,y_loc,4))
    {
        setVisible(true);
        return;
    }

    if(m_world->encounterobject(IID_PLAYER,x_loc,y_loc,3)&&permanent==true)
    {
        setDead();
        m_world->playSound(SOUND_GOT_GOODIE);
        m_world->increaseScore(10);
        m_world->setgold('+',1);
    }
    if(permanent==false&&m_world->encounterprotestor(x_loc, y_loc, 3)&&m_tick!=0)
    {
        setDead();
        m_world->playSound(SOUND_PROTESTER_FOUND_GOLD);
    }
    if(!permanent)
    {
        m_tick--;
        if(m_tick==0)
        setDead();
        
    }
  
    
}

///squirt class///
squirt::squirt(StudentWorld*world,int x,int y, Direction dir)
:Actor(world,IID_WATER_SPURT,x,y,dir,1,1),m_travel(4){}

squirt::~squirt(){}

void squirt::doSomething()
{
    if(!isAlive())
        return;
    Direction dir=getDirection();
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    if(m_world->encounterprotestor(x_loc, y_loc, 3))
        setDead();
       while(m_travel>0)
    {
        //if(dir==up)
            if(dir==up && !m_world->Boulderthere(x_loc, y_loc)&&!m_world->Dirtoverlap(x_loc,y_loc))
            {
                y_loc++;
                moveTo(x_loc, y_loc);
            }
        //if(dir==down)
            if(dir==down && !m_world->Boulderthere(x_loc, y_loc)&&!m_world->Dirtoverlap(x_loc,y_loc))
            {
                y_loc--;
                moveTo(x_loc, y_loc);
            }
        //if(dir==left)
            if(dir==left && !m_world->Boulderthere(x_loc, y_loc)&&!m_world->Dirtoverlap(x_loc,y_loc))
            {
                x_loc--;
                moveTo(x_loc, y_loc);
            }
        //if(dir==right)
            if(dir==right && !m_world->Boulderthere(x_loc, y_loc)&&!m_world->Dirtoverlap(x_loc,y_loc))
            {
                x_loc++;
                moveTo(x_loc, y_loc);
                std::cout << 1 << std::endl;
            }

        m_travel--;
        return;
        
    }
    setDead();//problem; graphic problem
}
///Barrel class//
Barrel::Barrel(StudentWorld*world,int x, int y)
:Actor(world,IID_BARREL,x,y,right,1,2),permanent(true)
{setVisible(!permanent);}

Barrel::~Barrel()
{}

void Barrel::doSomething()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world= getworld();
    if(!isAlive())
        return;
    if(!isVisible()&&m_world->encounterobject(IID_PLAYER,x_loc,y_loc,4))
    {
        setVisible(true);
        return;
    }
    if(m_world->encounterobject(IID_PLAYER,x_loc,y_loc,3))
    {
        setDead();
        m_world->playSound(SOUND_FOUND_OIL);
        m_world->increaseScore(1000);
        m_world->addoil();
    }
    
}
//sonar class
Sonar::Sonar(StudentWorld*world,int x,int y)
:Actor(world, IID_SONAR,x,y,right,1,2)
{
    m_ticks=fmax(100, 300-10*world->getLevel());
}
Sonar::~Sonar(){}
void Sonar::doSomething()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    if(!isAlive())
        return;
    if(m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 3))
    {
        setDead();
        m_world->playSound(SOUND_GOT_GOODIE);
        m_world->addsonar('+');
        m_world->increaseScore(75);
    }
    m_ticks--;
    if(m_ticks==0)
        setDead();
}
//water class//
Water::Water(StudentWorld*world,int x,int y)
:Actor(world,IID_WATER_POOL,x,y,right,1,2)
{
    m_ticks=fmax(100, 300-10*world->getLevel());
}
Water::~Water(){}

void Water::doSomething()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();

    if(!isAlive())
        return;
    if(m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 3))
    {
        setDead();
        m_world->playSound(SOUND_GOT_GOODIE);
        m_world->changewater('+');
        m_world->increaseScore(100);
    }
    m_ticks--;
    if(m_ticks==0)
        setDead();

}
////protestor class//
Protestor::Protestor(StudentWorld*world,int ID,int x,int y)
:Actor(world,ID,x,y,left,1,0),hitpoint(5),leavefield(false),shout(false),shoutcount(0),turn(0),chase(false),tickToPause(15*fmax(0, 3-(world->getLevel()/4))),dead(false)
{
    SquareToMove=rand()%53+8;
    m_tick=fmax(0, 3-(world->getLevel()/4));
}
Protestor::~Protestor()
{}
void Protestor::doSomething()
{
    
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    annoyprotestor();

    //std::cerr<<turn<<std::endl;
    if(m_tick>0)
    {
        m_tick--;
        return;
    }

    turn++;
    shoutcount++;
    if(m_tick<=0)
    {
    if(leavefield==true)
     {
        if(x_loc==60&&y_loc==60)
            setDead();
        else
         {
             int x;int y;
             m_world->exitmaze(x_loc, y_loc,60,60,path,x,y);
             changeDirection(x_loc, y_loc, x, y);
             if(x<=60&&x>=0&&y<=60&&y>=0)
             {
                 moveTo(x, y);
                 m_tick=fmax(0, 3-(m_world->getLevel()/4));
             }             
             return;
         }
     }

     if(m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 4))
    {
        if(shoutcount>=15)
        {
            
                m_tick=tickToPause;
                shoutcount=15;
                m_world->playSound(SOUND_PROTESTER_YELL);
                m_world->killPlayer(20);
                return;


        }
    }
       
    if(m_world->getplayerdirection(x_loc, y_loc)!=none&&!m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 4)&&m_world->canmove(x_loc,y_loc))
    {
            Direction dir=m_world->getplayerdirection(x_loc, y_loc);
            setDirection(dir);
            if(dir==right)
            {x_loc++;moveTo(x_loc, y_loc);}
            if(dir==up)
            {y_loc++;moveTo(x_loc,y_loc);}
                
            if(dir==down)
            {y_loc--;moveTo(x_loc,y_loc);}
            if(dir==left)
            {x_loc--;moveTo(x_loc, y_loc);}

            SquareToMove=0;
        m_tick=fmax(0, 3-(m_world->getLevel()/4));

            return;
            
        
    }//end else if
    if (!m_world->visualonPlayer(x_loc, y_loc))
    {
       // std::cerr<<SquareToMove<<std::endl;
        SquareToMove--;
        
        if(SquareToMove<=0)
        {
            Direction dir;

            dir=m_world->randDirection();
        while(!m_world->movable(x_loc,y_loc,dir))
        {
            dir=m_world->randDirection();
            //std::cerr<<"while loop"<<std::endl;

        }
            setDirection(dir);
            SquareToMove=rand()%53+8;
        }
    }
    
    if(m_world->intersection(x_loc, y_loc,getDirection())&&turn>=200)
    {
        std::cout<<"exceuted";
        Direction dir=getDirection();
        setDirection(m_world->pickperpendir(x_loc, y_loc, dir));
        SquareToMove=rand()%53+8;
        turn=0;
    }

    Direction dir=getDirection();
        moveProtestor(x_loc, y_loc, dir, SquareToMove);
 
    
    }

    
}
//AnnoyProtestor
void Protestor::annoyprotestor()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    if(dead==false)
    {
    if(m_world->encounterobject(IID_GOLD,x_loc,y_loc,3))
    {
        std::cerr<<"annoy";
        m_world->increaseScore(25);

        leavefield=true;
        dead=true;
        return;
    }
    if(m_world->encounterobject(IID_WATER_SPURT, x_loc, y_loc, 3))
    {
        hitpoint-=2;
        std::cerr<<"hit:"<<hitpoint<<std::endl;
        m_world->playSound(SOUND_PROTESTER_ANNOYED);
        m_world->increaseScore(100);
        m_tick = fmax(50, 100 - m_world->getLevel() * 10);
        return;
    }
    if(m_world->encounterobject(IID_BOULDER, x_loc, y_loc, 3))
    {
        hitpoint-=100;
        m_world->playSound(SOUND_PROTESTER_ANNOYED);
        m_world->increaseScore(500);
        m_tick = fmax(50, 100 - m_world->getLevel() * 10);
        return;
    }
    if(hitpoint<=0)
    {
        leavefield=true;
        dead=true;

        m_world->increaseScore(100);
        m_world->playSound(SOUND_PROTESTER_GIVE_UP);
        m_tick=0;
        hitpoint=1;
    }
    }

}
void Protestor::changeDirection(int curx,int cury,int destx,int desty)
{
    if(curx+1==destx&&cury==desty)
        setDirection(right);
    if(curx-1==destx&&cury==desty)
        setDirection(left);
    if(curx==destx&&cury-1==desty)
        setDirection(down);
    if(curx==destx&&cury+1==desty)
        setDirection(up);
}
void Protestor::moveProtestor(int x_loc, int y_loc, GraphObject::Direction dir, int &squaretomove)
{
    StudentWorld*m_world=getworld();
    if(dir==right&&m_world->movable(x_loc,y_loc,right))
    {
        x_loc++;
        moveTo(x_loc, y_loc);
        m_tick=fmax(0, 3-(m_world->getLevel()/4));

        
    }
    else if(dir==up&&m_world->movable(x_loc,y_loc,up))
    {
        y_loc++;
        moveTo(x_loc,y_loc);
    }
    
    else if(dir==down&&m_world->movable(x_loc,y_loc,down))
    {
        y_loc--;
        moveTo(x_loc,y_loc);
    }
    else if(dir==left&&m_world->movable(x_loc,y_loc,left))
    {
        x_loc--;
        moveTo(x_loc, y_loc);
    }
    else if (!m_world->movable(x_loc,y_loc,dir))
    {
        SquareToMove=0;
    }
    m_tick=fmax(0, 3-(m_world->getLevel()/4));
    return;
    

}
//hardcore Protestor
///////////////////////////////////////////////////////////////////////////
hardcoreProtestor::hardcoreProtestor(StudentWorld*world,int ID,int x,int y)
:Protestor(world,ID,x,y)
{
    hitpoint = 20;
    SquareToMove=rand()%53+8;
    m_tick=fmax(0, 3-(world->getLevel()/4));
}


hardcoreProtestor::~hardcoreProtestor()
{}

void hardcoreProtestor::doSomething()
{
    
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    
    annoyprotestor();
    
    //std::cerr<<turn<<std::endl;
    if(m_tick>0)
    {
        m_tick--;
        return;
    }
    
    turn++;
    shoutcount++;
    if(m_tick<=0)
    {
        if(leavefield==true)
        {
            if(x_loc==60&&y_loc==60)
                setDead();
            else
            {
                int x;int y;
                m_world->exitmaze(x_loc, y_loc,60,60,path,x,y);
                changeDirection(x_loc, y_loc, x, y);
                if(x<=60&&x>=0&&y<=60&&y>=0)
                {
                moveTo(x, y);
                m_tick=fmax(0, 3-(m_world->getLevel()/4));
                }
                return;
            }
        }
        
        if(m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 4))
        {
            if(shoutcount>=15)
            {
                m_tick=tickToPause;
                shoutcount=15;
                m_world->playSound(SOUND_PROTESTER_YELL);
                m_world->killPlayer(20);
                return;
            }
        }
        
        if(!m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 4))
        {
            int x1;int y1;
            int playerX; int playerY;
            m_world->getPlayerloc(playerX, playerY);
//            std::cerr<<"playerX"<<playerX<<"playerY"<<playerY<<std::endl;
            int level= m_world->exitmaze(x_loc, y_loc, playerX, playerY, path,x1,y1);
            std::cerr<<"playerX"<<x1<<"playerY"<<y1<<std::endl;
            int M = 16 + m_world->getLevel() * 2;
            if(M>=level)
            {

                std::cerr<<"level:"<<level<<std::endl;
                changeDirection(x_loc, y_loc, x1, y1);
                //std::cerr<<"playerX"<<x1<<"playerY"<<y1<<std::endl;
                if(x1<=60&&x1>=0&&y1<=60&&y1>=0)
                {
                    moveTo(x1,y1);
                    m_tick=fmax(0, 3-(m_world->getLevel()/4));
                }
                return;
            }
       }

        if(m_world->getplayerdirection(x_loc, y_loc)!=none&&!m_world->encounterobject(IID_PLAYER,x_loc, y_loc, 4)&&m_world->canmove(x_loc,y_loc))
        {
            Direction dir=m_world->getplayerdirection(x_loc, y_loc);
            setDirection(dir);
            if(dir==right)
            {x_loc++;moveTo(x_loc, y_loc);}
            if(dir==up)
            {y_loc++;moveTo(x_loc,y_loc);}
            
            if(dir==down)
            {y_loc--;moveTo(x_loc,y_loc);}
            if(dir==left)
            {x_loc--;moveTo(x_loc, y_loc);}
            
            SquareToMove=0;
            m_tick=fmax(0, 3-(m_world->getLevel()/4));
            
            return;
            
            
        }//end else if
        if (!m_world->visualonPlayer(x_loc, y_loc))
        {
            // std::cerr<<SquareToMove<<std::endl;
            SquareToMove--;
            
            if(SquareToMove<=0)
            {
                Direction dir;
                
                dir=m_world->randDirection();
                while(!m_world->movable(x_loc,y_loc,dir))
                {
                    dir=m_world->randDirection();
                    //std::cerr<<"while loop"<<std::endl;
                    
                }
                setDirection(dir);
                SquareToMove=rand()%53+8;
            }
        }
        
        if(m_world->intersection(x_loc, y_loc,getDirection())&&turn>=200)
        {
            std::cout<<"exceuted";
            Direction dir=getDirection();
            setDirection(m_world->pickperpendir(x_loc, y_loc, dir));
            SquareToMove=rand()%53+8;
            turn=0;
        }
        
        Direction dir=getDirection();
        moveProtestor(x_loc, y_loc, dir, SquareToMove);
        
    }
    
}

void hardcoreProtestor::annoyprotestor()
{
    int x_loc=getX();
    int y_loc=getY();
    StudentWorld*m_world=getworld();
    if(m_world->encounterobject(IID_GOLD,x_loc,y_loc,3))
    {
        std::cerr<<"annoy";
        
        m_tick = fmax(50, 100 - m_world->getLevel() * 10);
        m_world->increaseScore(50);

        return;
    }
    if(m_world->encounterobject(IID_WATER_SPURT, x_loc, y_loc, 3))
    {
        hitpoint-=2;
        std::cerr<<"hit:"<<hitpoint<<std::endl;
        m_world->playSound(SOUND_PROTESTER_ANNOYED);
        m_world->increaseScore(25);
        m_tick = fmax(50, 100 - m_world->getLevel() * 10);
        return;
    }
    if(m_world->encounterobject(IID_BOULDER, x_loc, y_loc, 3))
    {
        hitpoint-=100;
        m_world->playSound(SOUND_PROTESTER_ANNOYED);
        m_world->increaseScore(500);
        m_tick = fmax(50, 100 - m_world->getLevel() * 10);
        return;
    }
    if(hitpoint<=0)
    {
        leavefield=true;
        m_world->increaseScore(250);
        m_world->playSound(SOUND_PROTESTER_GIVE_UP);
        m_tick=0;
        hitpoint=1;
    }

}
//
