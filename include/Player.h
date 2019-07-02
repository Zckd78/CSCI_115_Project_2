/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 * Extended by Zachary Scott
 * for CSCI 115 - Project 02
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <Structures.h>

// Player class
class Player : public Block
{
public:
    Player();
    ~Player();

    void Draw();                        // Draw Player image
    void DrawArrow();                   // Draw Arrow
    void ShootArrow();                  // Shoot arrow movement

    void Init(int,int);                 // initialize the player with grid size, image, frames
    void MovePlayer(DIRECTION);            // move Player left,right,up,down
    void TurnPlayer(DIRECTION);            // turn Player left,right,up,down
    void Animate();                     // Animate sprite
    void LoadArrowImage(char *);        // set an image for the Arrow
    void SetupTexture(char*);

    // Location
    GridLoc GetPlayerLoc();            // player current location
    GridLoc GetArrowLoc();             // arrow current location

    // Player Status
    DIRECTION playerDir;                   // direction of player face
    int arrowsLeft = 2;
    bool arrowStatus;                  // arrow is active or not
    bool livePlayer;                   // Player live or dead

private:
    // Texture Related
    float xmax, xmin,ymax,ymin;        // animation texture map
    GLuint arrowTex;                   // arrow image handler

    // Arrow Vars
    int arrXdir;                       // arrow direction x;
    int arrYdir;                       // arrow direction y
    float arrAngle;                    // animation direction of the arrow
    loc arrowLoc;                      // viewport location of the arrow

    // Player Location
    loc Converter(int, int);           // convert Grid location to viewport

    // Animation
    int frames;                        // Number of frames for animation
    int stepsPerMove;                  // animation steps
    float moveDis=0;                   // Moving distance for animation
    bool mUp,mDown,mLeft,mRight;       // move direction
    bool fUp,fDown,fLeft,fRight;       // facing direction
    float t;                           // Ticker for animation
    void timer();                      // Timer for animation

};

#endif // PLAYER_H
