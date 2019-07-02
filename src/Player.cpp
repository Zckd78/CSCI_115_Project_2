#include "Player.h"


Timer *T = new Timer();                             // Set Timer for animation

Player::Player()
{
    //ctor
    type = PLAYER;
    playerDir = DOWN;
    mUp=mDown=mLeft=mRight=false;
    fUp=fDown=fLeft=fRight=false;
    arrowLoc.x = 0.2;
    arrowLoc.y =0.5;
    arrAngle =0;
    arrowStatus = false;
    livePlayer = true;
}

Player::~Player()
{
    //dtor
}


void Player::Init(int gSize,int frams)
{
    frames = frams;
    gridSize = gSize;

    xmax =1/(float)frames;
    ymax =0.25;
    xmin =0;
    ymin =0;

    unitWidth = (float)2/gridSize;
    stepsPerMove = frames;

    T->Start();
    Loc.x= -unitWidth;
    Loc.y= -unitWidth;

    // t= unitWidth/stepsPerMove;
    t = (float)(2.0/gridSize)/frames;

    SetupTexture("images/p.png");
    LoadArrowImage("images/arrow.png");
}

void Player::SetupTexture(char *FileName)
{
    Tex = TextureLoader(FileName);
}

void Player::DrawArrow()
{
    if(arrowStatus)
    {
        glColor3f(1.0,1.0,1.0);

        // Animate the Arrow?
        if(T->GetTicks()>1)
        {
            if(arrowLoc.x>=-1 && arrowLoc.x<=1)
                arrowLoc.x += t*arrXdir;
            else arrowStatus = false;
            if(arrowLoc.y>=-1 && arrowLoc.y<=1)
                arrowLoc.y += t*arrYdir;
            else arrowStatus = false;
            T->Reset();
        }
        glTranslatef(arrowLoc.x,arrowLoc.y,0.0);
        glRotated(arrAngle,0,0,1);
        glBindTexture(GL_TEXTURE_2D,arrowTex);
        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);
        glEnd();
    }
}

void Player::ShootArrow()
{
    if(livePlayer)
    {
        if(!arrowStatus && arrowsLeft>0)
        {
            arrowStatus=true;

            if(playerDir == LEFT)
            {
                arrAngle = -90;
                arrXdir=-1;
                arrYdir=0;
                arrowLoc.x = Loc.x-unitWidth/2;
                arrowLoc.y = Loc.y;
            }
            if(playerDir == RIGHT)
            {
                arrAngle =90;
                arrXdir=1;
                arrYdir=0;
                arrowLoc.x = Loc.x+unitWidth/2;
                arrowLoc.y = Loc.y;
            }
            if(playerDir == UP)
            {
                arrXdir=0;
                arrYdir=1;
                arrAngle =180;

                arrowLoc.y = Loc.y+unitWidth/2;
                arrowLoc.x = Loc.x;
            }
            if(playerDir == DOWN)
            {
                arrXdir=0;
                arrYdir=-1;
                arrAngle= 0;

                arrowLoc.y = Loc.y-unitWidth/2;
                arrowLoc.x = Loc.x;
            }
            // Expended an arrow
            arrowsLeft--;
        }
    }
}


void Player::LoadArrowImage(char* fileName)
{
    arrowTex = TextureLoader(fileName);
}

GridLoc Player::GetArrowLoc()
{
    GridLoc val;

    val.x = (int)(ceil((arrowLoc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((arrowLoc.y +(1-unitWidth))/unitWidth));

    return val;
}

void Player::Draw()
{
    if(livePlayer)
    {
        glPushMatrix();
        Animate();

        glColor3f(1.0,1.0,1.0);

        glTranslatef(Loc.x,Loc.y,0.0);

        glBindTexture(GL_TEXTURE_2D,Tex);
        glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

        glBegin(GL_QUADS);
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



void Player::MovePlayer(DIRECTION dir)
{
    if(moveDis<=0)
    {
        if(dir == LEFT)
        {
            playerDir = LEFT;
            mLeft = fLeft = true;
            mDown=mUp=mRight= false;
            fDown=fUp=fRight= false;
        }

        else if(dir == RIGHT)
        {
            playerDir = RIGHT;
            mRight = fRight= true;
            mDown=mLeft=mUp=false;
            fDown=fLeft=fUp=false;
        }

        else if(dir == UP)
        {
            playerDir = UP;
            mUp = fUp = true;
            mDown=mLeft=mRight=false;
            fDown=fLeft=fRight=false;
        }

        else if(dir == DOWN)
        {
            playerDir = DOWN;
            mDown =fDown = true;
            mUp=mLeft=mRight=false;
            fUp=fLeft=fRight=false;
        }
    }
}

void Player::TurnPlayer(DIRECTION dir)
{
    if(dir == LEFT)
    {
        playerDir = LEFT;
        fLeft = true;
        fDown=fUp=fRight= false;
    }

    else if(dir == RIGHT)
    {
        playerDir = RIGHT;
        fRight= true;
        fDown=fLeft=fUp=false;
    }

    else if(dir == UP)
    {
        playerDir = UP;
        fUp = true;
        fDown=fLeft=fRight=false;
    }

    else if(dir == DOWN)
    {
        playerDir = DOWN;
        fDown = true;
        fUp=fLeft=fRight=false;
    }
}


void Player::Animate()
{
    // Handle turning character
    if(fUp)
    {
        ymin =0.25;
        ymax =0.5;
    }

    else if(fDown)
    {
        ymin =0.0;
        ymax =0.25;
    }
    else if(fLeft)
    {
        ymin =0.75;
        ymax =1.0;
    }

    else if(fRight)
    {
        ymin =0.5;
        ymax =0.75;
    }

    if(moveDis < unitWidth)
    {
        // Handle the walking animation
        if(T->GetTicks()<600)
        {
            if(mUp)
            {
                if(Loc.y< 1-unitWidth/2)
                {
                    Loc.y += t;
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

            else if(mDown)
            {
                if(Loc.y > -1+unitWidth/2)
                {
                    Loc.y -= t;
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
            else if(mLeft)
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

            else if(mRight)
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
        else T->Reset();
    }
    else
    {
        moveDis =0;
        mDown=mUp=mLeft=mRight=false;
        fDown=fUp=fLeft=fRight=false;
    }
}



GridLoc Player::GetPlayerLoc()
{
    GridLoc val;

    val.x = (int)(ceil((Loc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((Loc.y +(1-unitWidth))/unitWidth));

    return val;

}
