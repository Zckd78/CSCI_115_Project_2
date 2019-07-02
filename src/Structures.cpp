#include "Structures.h"

using namespace std;


loc::loc()
{
    x=0;
    y=0;
}

loc::loc(int pX, int pY)
{
    x=pX;
    y=pY;
}

GridLoc::GridLoc()
{
    x=0;
    y=0;
}

GridLoc::GridLoc(int pX, int pY)
{
    x=pX;
    y=pY;
}

bool GridLoc::operator==(const GridLoc pLoc)
{
    return ( this->x == pLoc.x && this->y == pLoc.y );
}

bool GridLoc::operator!=(const GridLoc pLoc)
{
    return ( this->x != pLoc.x || this->y != pLoc.y );
}


Block::Block()
{
    Loc.x = 0;
    Loc.y = 0;
}

Block::Block(int x, int y)
{
    Loc.x = x;
    Loc.y = y;
}

Block::~Block()
{

}
void Block::Draw()
{
    return;
}

void Block::Init(int x, int y)
{
    return;
}

void Block::SetupTexture(char* file)
{
    Tex = TextureLoader(file);
}

void Block::Place(int x, int y)
{
    gLoc.x = x;
    gLoc.y = y;
    gDest.x = x;
    gDest.y = y;

    Loc.x =  Converter(x,y).x;
    Loc.y =  Converter(x,y).y;
}

loc Block::Converter(int x, int y)
{
    loc val;
    x+=1;
    y+=1;

    val.x = -1-unitWidth/2+(unitWidth)*x;
    val.y = -1-unitWidth/2+(unitWidth)*y;
    return val;
}

GridLoc Block::GetGridPos()
{
    GridLoc val;

    val.x = (int)(ceil((Loc.x +(1-unitWidth))/unitWidth));
    val.y = (int)(ceil((Loc.y +(1-unitWidth))/unitWidth));

    return val;
}


Chest::Chest()
{
    type = CHEST;
}

void Chest::Init(int grid, int)
{
    gridSize = grid;
    unitWidth = (float)2/grid;
    SetupTexture("images/chest.png");
}

void Chest::Draw()
{
    if(!collected)
    {
        glColor3f(1.0,1.0,1.0);

        glTranslatef(Loc.x,Loc.y,1.0);

        glBindTexture(GL_TEXTURE_2D,Tex);
        glScaled(1.0/(float)(gridSize+5),1.0/(float)(gridSize+5),1.0);

        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
        glEnd();
    }
    return;
}


Arrows::Arrows()
{
    type = ARROWS;
}


void Arrows::Init(int grid, int)
{
    gridSize = grid;
    unitWidth = (float)2/grid;
    SetupTexture("images/quiver.png");
}


void Arrows::Draw()
{
    if(!collected)
    {
        glColor3f(1.0,1.0,1.0);

        glTranslatef(Loc.x,Loc.y,1.0);
        // glRotated(-spin,0,0,1);

        glBindTexture(GL_TEXTURE_2D,Tex);
        glScaled(1.0/(float)(gridSize),1.0/(float)(gridSize),1.0);

        glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex3f(1,-1,0.0f);

        glTexCoord2f(0,0);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(1,0);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(1,1);
        glVertex3f(-1,-1,0.0f);
        glEnd();
    }
    return;
}

Wall::Wall()
{
    type = WALL;
}

Wall::~Wall()
{
    //dtor
}

void Wall::Init(int grid,int)
{
    gridSize = grid;
    unitWidth = (float)2/grid;
    SetupTexture("images/wall.png");
}

void Wall::Draw()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,Tex);

    glPushMatrix();
    glTranslatef(Loc.x,Loc.y,0.0);

    glScalef(1.0/(float)gridSize,1.0/(float)gridSize,1);


    glBegin(GL_POLYGON);
    glTexCoord2f(0,0);
    glVertex3f(1,1,0.0f);

    glTexCoord2f(1,0);
    glVertex3f(-1,1,0.0f);

    glTexCoord2f(1,1);
    glVertex3f(-1,-1,0.0f);

    glTexCoord2f(0,1);
    glVertex3f(1,-1,0.0f);
    glEnd();
    glPopMatrix();

}

PlayBoard::PlayBoard()
{
    // Default size of 40
    PlayBoard(40);
    boardSize = 40;
}

PlayBoard::~PlayBoard()
{
    // Go through and delete every element
    for(int i=0; i <= boardSize; i++)
    {
        // Then go through each item in that row, and make those all new Blocks
        for(int j=0; j <= boardSize; j++)
        {
            delete Grid[i][j];
        }
    }
    Grid = nullptr;
}


PlayBoard::PlayBoard(int s)
{
    // Set size of board
    boardSize = s;
    // Grid is a new Block array
    Grid = new Block**[boardSize];
    // Set each element spot to another Block array
    for(int i=0; i < boardSize; i++)
    {
        // Make each i column, a point to an array of Block Pointers
        Grid[i] = new Block*[boardSize];
        // Then go through each item in that row, and make those all new Blocks
        for(int j=0; j < boardSize; j++)
        {
            int xVal = boardSize - j;
            int yVal = boardSize - i;

            Grid[i][j] = new Block(xVal,yVal);
        }
    }
};

