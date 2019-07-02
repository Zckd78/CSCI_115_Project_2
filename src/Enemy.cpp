/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 * Extended by Zachary Scott
 * for CSCI 115 - Project 02
 */

#include <Enemy.h>
#include <iostream>

using namespace std;



Timer *T1 = new Timer();                             // Set Timer for animation
Enemy::Enemy()
{
    //ctor
    type = ENEMY;
    Loc.x=0;
    Loc.y=0;

    xmax =1;
    ymax =0.25;
    xmin =0;
    ymin =0;
    up= down = left=right=false;
    alive = true;

}

Enemy::~Enemy()
{
    //dtor
}

void Enemy::Init(int grid,int numFrames)
{
    gridSize = grid;
    frames = numFrames;
    xmax =1/(float)frames;
    T1->Start();
    t = (float)(2.0/grid)/frames;
    unitWidth = (float)2.0/gridSize;
    SetupTexture("images/e.png");
}
void Enemy::SetupTexture(char* FileName)
{
    Tex = TextureLoader(FileName);
}

void Enemy::Draw()
{
    if(alive)
    {
        glColor3f(1.0,1.0,1.0);
        glBindTexture(GL_TEXTURE_2D,Tex);

        glPushMatrix();
        Animate();
        glTranslatef(Loc.x,Loc.y,0.0);

        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

        glBegin(GL_POLYGON);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
        glEnd();
        glPopMatrix();
    }
}


void Enemy::PlaceEnemy(int x, int y)
{
    unitWidth = 2.0/gridSize;
    x+=1;
    y+=1;
    Loc.x =  -1-unitWidth/2+(unitWidth)*x;
    Loc.y =  -1-unitWidth/2+(unitWidth)*y;
}

void Enemy::MoveEnemy(DIRECTION dir)
{
    if(moveDis<=0)
    {
        if(dir==UP)
        {
            up=true;
            down=left=right=false;
        }
        else if(dir==DOWN)
        {
            down=true;
            up=left=right=false;
        }
        else if(dir==LEFT)
        {
            left=true;
            down=up=right=false;
        }
        else if(dir==RIGHT)
        {
            right=true;
            down=left=up=false;
        }
        else
        {
            up=left=right=false;
        }
    }
}

void Enemy::Animate()
{
    if(moveDis < unitWidth)
    {
        if(T1->GetTicks()<1000)
        {
            if(up)
            {
                if(Loc.y< 1-unitWidth/2)
                {
                    Loc.y += t;
                }
                moveDis +=t;

                xmin +=1/(float)frames;
                xmax +=1/(float)frames;
                ymin =0.0;
                ymax =0.25;
                if(xmax>1)
                {
                    xmax =1/(float)frames;
                    xmin =0;
                }
            }

            else if(down)
            {
                if(Loc.y > -1+unitWidth/2)
                {
                    Loc.y -= t;
                }
                moveDis +=t;

                xmin +=1/(float)frames;
                xmax +=1/(float)frames;
                ymin =0.25;
                ymax =0.5;

                if(xmax>1)
                {
                    xmax =1/(float)frames;
                    xmin =0;
                }

            }
            else if(left)
            {
                if(Loc.x>-1+unitWidth/2)
                {
                    Loc.x -= t;
                }
                moveDis +=t;

                xmin +=1/(float)frames;
                xmax +=1/(float)frames;
                ymin =0.75;
                ymax =1.0;

                if(xmax>1)
                {
                    xmax =1/(float)frames;
                    xmin =0;
                }
            }

            else if(right)
            {
                if(Loc.x<1-unitWidth/2)
                {
                    Loc.x += t;
                }
                moveDis +=t;

                xmin +=1/(float)frames;
                xmax +=1/(float)frames;
                ymin =0.5;
                ymax =0.75;

                if(xmax>1)
                {
                    xmax =1/(float)frames;
                    xmin =0;
                }
            }

        }
        else T1->Reset();
    }
    else
    {
        moveDis =0;
        down=up=left=right=false;
    }
}


GridLoc Enemy::GetEnemyLoc()
{
    GridLoc val;
    val.x = (int)(ceil((Loc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((Loc.y +(1-unitWidth))/unitWidth));

    return val;
}

