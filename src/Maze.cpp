/*
    Written by Zachary Scott
    for CSCI 115 - Project 02
*/

#include "Maze.h"

Maze::Maze()
{
    mCtrl = new MapController(20);
}

// Start the Maze with the provided size
Maze::Maze(int s)
{
    mCtrl = new MapController(s);
}

Maze::~Maze()
{
    //dtor
    cout << "|==> Deconstructing Maze..." << endl;
    delete mCtrl;
}
void Maze::StartLevel(int l)
{
    // Recreated the Map Controller when we reload the level.
    mCtrl = new MapController(20);
    switch(l)
    {
    case 1:
        currentLevel = 1;
        mCtrl->LoadMap("maps/Level1.map");
        break;
    case 2:
        currentLevel = 2;
        mCtrl->LoadMap("maps/Level2.map");
        break;
    case 3:
        currentLevel = 3;
        mCtrl->LoadMap("maps/Level3.map");
        break;
    case 4:
        currentLevel = 4;
        mCtrl->LoadMap("maps/Level4.map");
        break;

    }
}

void Maze::TryMovePlayer(DIRECTION dir)
{
    // If we're not colliding with a Wall
    if(!mCtrl->DoesCollide(dir))
    {
        // Move the player, which updates their Destination GridLoc
        mCtrl->MovePlayer(dir);

        // Test if we acquired arrows
        mCtrl->CheckPickupArrows();

        // Win Condition satisfied?
        if (mCtrl->CheckWin())
        {
            signalWin = true;
            T.Start();
            return;
        }


        // Testing if the player walked into the enemies
        if (mCtrl->CheckDeath())
        {
            signalLose = true;
            T.Start();
            return;
        }

        // Update Enemies
        mCtrl->AwakenEnemies();


        // Did the player die?
        if (mCtrl->CheckDeath())
        {
            signalLose = true;
            T.Start();
            return;
        }

        // Finally update everyone's location
        mCtrl->UpdateAllLocations();

        // Debugging
        // mCtrl->DisplayBoard();

    }

}

void Maze::TurnPlayer(DIRECTION dir)
{
    mCtrl->GetPlayer()->TurnPlayer(dir);
}

void Maze::TryShootArrow()
{
    mCtrl->GetPlayer()->ShootArrow();
}


void Maze::Display()
{

    // Switch to the Intro after a bit
    if (state == START and T.GetTicks() > 1500)
    {
        state = INTRO;
        T.Stop();
        T.Reset();
    }

    // Quit after a few seconds when the game is finished
    if (state == ENDGAME and T.GetTicks() > 3500)
    {
        exit(0);
    }

    // Handle delayed state changes in the level
    if( state == LEVEL && T.isStarted() && T.GetTicks() > 250)
    {
        if(signalLose)
        {
            state = LOSE;
        }

        // Reset Timer and Signals
        signalLose = false;
        T.Stop();
    }

    switch(state)
    {
    case START:
        // Start timer, change screen soon.
        T.Start();
        mCtrl->DrawStart();
        break;
    case INTRO:
        mCtrl->DrawIntro();
        break;
    case SELECT:
        mCtrl->DrawLevelSelect();
        break;
    case MENU:
        // Draw the Menu on top of the board
        mCtrl->DrawBoard();
        mCtrl->DrawMenu();
        break;
    case LEVEL:
        // Checking Win condition on Display in case we won by killing all the enemies
        if (mCtrl->CheckWin())
            state = WIN;
        mCtrl->DrawBoard();
        break;
    case WIN:
        mCtrl->DrawLevelComplete();
        break;
    case LOSE:
        mCtrl->DrawGameOver();
        break;
    case ENDGAME:
        T.Start();
        mCtrl->DrawEndGame();
        break;

    }

}
