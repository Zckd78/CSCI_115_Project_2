/*
    Written by Zachary Scott
    for CSCI 115 - Project 02
*/

#ifndef MAZE_H
#define MAZE_H

#include <MapController.h>

// Enumerator to decide what state the game is in.
// Many functions in the Maze class use this.
enum GameState { START, INTRO, SELECT, MENU, LEVEL, WIN, LOSE, ENDGAME };

/*
Maze Class
    The Maze provides an abstract interface to the underlying MapController.
    The MapController provides the bulk of the functionality of this project.
*/

class Maze
{
public:
    Maze();
    Maze(int);
    ~Maze();

    void StartLevel(int l);
    void TryMovePlayer(DIRECTION dir);
    void TurnPlayer(DIRECTION dir);
    void TryShootArrow();
    void Display();

    // Game State to be changed through actions, and key bindings
    GameState state = START;
    // Keeps track of the current level for restarting
    int currentLevel = 0;

private:
    MapController * mCtrl;

    // Signals a pending state change
    bool signalLose = false;
    bool signalWin = false;

    Timer T;

};

#endif // MAZE_H
