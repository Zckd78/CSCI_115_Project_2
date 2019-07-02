/*
    Written by Zachary Scott
    for CSCI 115 - Project 02
*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string.h>

#include<GL/gl.h>
#include<SOIL.h>

#include<iostream>
#include <Timer.h>
#include <math.h>       /* ceil */
#include <queue>
#include <TextureLoader.h>

// Program Control Vars
static bool DEBUG_MODE = false;
static bool MAP_TESTING_MODE = false;

class loc
{
public:
    loc();
    loc(int,int);

    float x;
    float y;
};

class GridLoc
{
public:
    GridLoc();
    GridLoc(int,int);
    bool operator==(const GridLoc);
    bool operator!=(const GridLoc);
    int x;
    int y;
};

enum BLOCK_TYPE { EMPTY, WALL, PLAYER, ENEMY, ARROWS, CHEST };

enum DIRECTION { UP, DOWN, LEFT, RIGHT, NONE };

static string DirectionToString(DIRECTION dir)
{
    if(dir == UP)
    {
        return "up";
    }
    else if(dir == DOWN)
    {
        return "down";
    }
    else if(dir == LEFT)
    {
        return "left";
    }
    else if(dir == RIGHT)
    {
        return "right";
    }
    else
    {
        return "";
    }
}

// Block struct to hold info about the space
// Block Types
class Block
{
public:
    // Functions
    Block();
    Block(int x, int y);
    // Functions to be overriden by children
    virtual ~Block();
    virtual void Draw();
    virtual void Init(int,int);
    // Shared Functions
    void SetupTexture(char*);
    void Place(int,int);
    loc Converter(int x, int y);
    GridLoc GetGridPos();

    // Variables
    // Type
    BLOCK_TYPE type = EMPTY;
    // Location
    int gridSize;
    loc Loc;
    // Current location in the Grid
    GridLoc gLoc;
    // Destination before movement occurs
    GridLoc gDest;

    // Texture
    GLuint Tex;
    // Sizing
    float unitWidth;
    // Misc
    bool collected = false;
};

class Chest : public Block
{
public:
    bool opened;
    Chest();
    void Draw();
    void Init(int, int);
};

class Arrows : public Block
{
public:
    Arrows();
    void Draw();
    void Init(int, int);

};

class Wall : public Block
{
public:
    Wall();
    ~Wall();

    void Draw();
    void Init(int, int);

};

// Holds the entire Grid of play.
class PlayBoard
{
public:
    // 2D Array container of Block pointers
    Block *** Grid;
    // Board dimensions
    int boardSize = 0;

    PlayBoard();
    PlayBoard(int s);
    ~PlayBoard();
};

// Used in Shortest Path Algorithm
struct NodePath
{
    // Current Node after taking all steps in Dirs
    Block * block;
    // Direction to take
    DIRECTION direction;
} ;


#endif // STRUCTURES_H
