/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 * Extended by Zachary Scott
 * for CSCI 115 - Project 02
 */

#ifndef ENEMY_H
#define ENEMY_H

#include <TextureLoader.h>
#include <Structures.h>
#include <time.h>
#include <thread>
#include <string>
#include <math.h>       /* ceil */

class Enemy : public Block
{
public:
    Enemy();                          // constructor
    virtual ~Enemy();                 // DeConstructor
    virtual void Init(int, int);     // Initialize Enemy

    // Location and Movement
    void PlaceEnemy(int,int);           // place enemy
    void MoveEnemy(DIRECTION);             // move Enemy left,right,up,down
    GridLoc GetEnemyLoc();              // Return Enemy current grid location

    // Display
    void SetupTexture(char * FileName);
    void Draw();                   // Draw Enemy with Animation
    void Animate();                     // Animate sprite

    // Vars
    int stepsPerMove;                   // Step count for animation
    bool alive;                          // Status of the Enemy

private:
    int frames;                        // number of frames generally
    float t;                           // steps for animation count

    float xmax, xmin,ymax,ymin;        // Animated Texture mapping
    bool up,down,left,right;           // move direction
    float moveDis=0;                   // Moving distance for animation
};

#endif // ENEMY_H
