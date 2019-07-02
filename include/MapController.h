/*
    Written by Zachary Scott
    for CSCI 115 - Project 02
*/

#ifndef MAPCONTROLLER_H
#define MAPCONTROLLER_H

#include <string.h>
#include <sstream>
#include <random>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include <Structures.h>
#include <Player.h>
#include <Enemy.h>

using namespace std;

/*
Map Controller
    Class to handle the Loading, Generation, and Saving of maps for our world
    This class will also serve to hold the current map loaded, determine available
    actions, and pass back checks on map rules.
*/

class MapController
{
public:

    MapController();
    MapController(int bSize);
    ~MapController();

    void OpenBoard(int);

    // Starts everything
    void Initialize();

    // Draw all Objects
    void DrawBoard();
    // Draw Board to the Console
    void DisplayBoard();

    // Display Setup Functions
    void InitializeGraphics();
    void LoadBackgroundImage(char *);   // set an image for the background
    void LoadStartImage(char *);
    void LoadDarknessImage(char *);
    void LoadGameOverImage(char *);
    void LoadEndGameImage(char *);
    void LoadLevelCompleteImage(char *);
    void LoadLevelSelectImage(char *);
    void LoadMenuImage(char *);
    void LoadIntroImage(char *);
    void DrawBackground();              // Draw background image
    void DrawStart();
    void DrawDarkness();
    void DrawGameOver();
    void DrawMenu();
    void DrawEndGame();
    void DrawLevelComplete();
    void DrawLevelSelect();
    void DrawIntro();
    void DrawGrid();                    // Draw Grid

    // Size and Location of Blocks
    int gridSize;

    // Loads the map from a file, and translating text into Blocks
    void LoadMap(string filePath);
    Block * FormBlock(char bl, int xPos, int yPos);
    string BlockForm(Block * bl);

    // Functions to retreive private items
    Player * GetPlayer();

    // Block Related
    Block * GetBlockAt(int xPos, int yPos);
    Block * GetBlockAt(GridLoc loc);
    Block * PeekBlock(Block * b, DIRECTION dir);
    void SwapGridLocations(GridLoc loc1, GridLoc loc2);
    void SwapGridLocations(int i, int j, int x, int y);
    void SwapBlocks(Block*& b1, Block*& b2);

    // Game Logic
    void MovePlayer(DIRECTION dir);
    void UpdateAllLocations();
    void UpdateLocation(Block* bl, Block* dest);
    void UpdateLocation(Enemy* bl, Block* dest);
    bool DoesCollide(DIRECTION dir);
    bool ValidMove(Block * b1, DIRECTION dir);
    bool CheckDeath();
    bool CheckWin();
    bool CheckPickupArrows();
    void CheckArrowCollision();

    // Enemy Related
    void AwakenEnemies();
    void ChasePlayer(Enemy * en, GridLoc pLoc);
    void FindValidMoves(Block* en, DIRECTION* moves, int* found);
    Enemy * GetEnemyAt(GridLoc loc);


private:

    // Map Loading Vars
    // Global index of the position
    int cIndex = 0;
    // Line number
    int cLine = 0;
    // Column number
    int cColumn = 0;

    // Index for the enemies
    int eInd =0;

    // File Hanlder
    fstream myfile;
    // Holds the current map in Text form
    string mapText;
    int mapLen=0;

    // Playboard Vars
    // Textures and Locations
    GLuint backgroundTexture;                       // Handler for background texture
    GLuint darknessTexture;
    GLuint startTexture;
    GLuint introTexture;
    GLuint gameOverTexture;
    GLuint endGameTexture;
    GLuint menuTexture;
    GLuint levelCompleteTexture;
    GLuint levelSelectTexture;
    float unitWidth;

    // Empty Block count
    int emptyCount = 0;
    // Create structure to hold nodes explored
    // Queue to track the moves yet to make
    // NodePath object holds the Block and direction it took to get there
    queue<NodePath> frontier;
    bool isExplored(GridLoc * explored, GridLoc gL);


    // Player
    Player * player;

    // Enemy Related
    int enemyIndex = 0;
    int enemyMax = 0;
    int enemyCount = 0;
    // Enemies (As an array)
    Enemy ** enemies;
    // Chest
    Chest * chest;
    // Arrows
    Arrows * arrows;

    // Contains all elements in The Grid
    PlayBoard * playboard;

    // Map Loading/Saving Functions
    void TextToMap();
};

#endif // MAPCONTROLLER_H
