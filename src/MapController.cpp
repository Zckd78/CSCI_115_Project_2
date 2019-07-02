/*
    Written by Zachary Scott
    for CSCI 115 - Project 02
*/

#include "MapController.h"

MapController::MapController()
{
    //Create a Random Playboard
    // MapController(20,4);
}

MapController::MapController(int bSize)
{
    OpenBoard(bSize);
    Initialize();
}


MapController::~MapController()
{
    //dtor
    cout << "|==> Deconstructing MapController..." << endl;
    delete playboard;

}

// Initialize the Grid and sizing
void MapController::OpenBoard(int bSize)
{
    playboard = new PlayBoard(bSize);

    gridSize = bSize;

    unitWidth = (float)2/gridSize;
}


// Used to Load Textures and create arrays
void MapController::Initialize()
{
    // Load Images and set textures
    InitializeGraphics();

    // Player Setup
    player = new Player();
    player->Init(gridSize,6);   // initialize player pass grid size,image and number of frames
    player->Place(5,5);

    // Create Array of Enemy Pointers
    enemies = new Enemy*[10];
}

// Draws the whole Level board
void MapController::DrawBoard()
{
    // Draw Background and the Grid
    glPushMatrix();
    DrawBackground();
    glPopMatrix();

    glPushMatrix();
    DrawGrid();
    glPopMatrix();

    // Draw the Arrow
    glPushMatrix();
    player->DrawArrow();
    glPopMatrix();

    // Check if the arrow has collided with something
    CheckArrowCollision();

    // Draw all Blocks
    for(int i=0; i < gridSize; i++)
    {
        for(int j=0; j < gridSize; j++)
        {
            Block * temp = playboard->Grid[i][j];

            if (temp != nullptr)
            {
                // Create a Block type object from the character
                glPushMatrix();
                temp->Draw();
                glPopMatrix();
            }

        } // END INNER LOOP
    } // END OUTER LOOP


    if(!MAP_TESTING_MODE)
    {
        // Add Darkness overlay
        glPushMatrix();
        DrawDarkness();
        glPopMatrix();
    }

}

// Debugging purposes, writes the board out in the console
void MapController::DisplayBoard()
{
    // Draw all Blocks
    for(int i=gridSize-1; i > 0; i--)
    {
        for(int j=0; j < gridSize; j++)
        {
            Block * temp = playboard->Grid[i][j];

            cout << BlockForm(temp) << "[" << temp->gLoc.x << "," << temp->gLoc.y << "]";

        } // END INNER LOOP
        cout << endl;
    } // END OUTER LOOP

    // Write out Important Locations

}


void MapController::DrawBackground()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,backgroundTexture);

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


void MapController::DrawGrid()
{
    float a;
    glColor4f(0.0,0.0,0.0,0.2);
    glDisable(GL_TEXTURE_2D);
    glPointSize(20);
    glBegin(GL_LINES);

    for(int i =0; i<gridSize; i++)
    {
        a = -1+(float)unitWidth*i;
        glVertex3f(a,1,-0.2);
        glVertex3f(a,-1,-0.2);
        glVertex3f(-1,a,-0.2);
        glVertex3f(1,a,-0.2);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}


void MapController::LoadMap(string filePath)
{
    cout << "|==> Loading Map File " << filePath << endl;

    // We also need to count the number of characters to determine the
    int cCount=0;
    bool counted=false;
    char inp;
    mapText = "";

    // Open file to read
    myfile.open(filePath,fstream::in);
    // Check if the file exists.
    if (!myfile.good())
    {
        cout << "|==> File " << filePath << " does not exist." << endl;
        return;
    }
    cout << "|==> Processing Map ... " << endl;

    // Create a buffer, and extract all chars
    stringstream buf;
    buf << myfile.rdbuf();

    // Set map text
    mapText = buf.str();

    // Test characters on the first line to get the Board size
    while (inp != '\n' && inp != '\r')
    {
        inp = mapText.at(cCount++);
    }

    // Close the file
    myfile.close();

    // Get the board size from file, minus one to offset the while loop
    gridSize = cCount-1;

    // Reset the Board
    OpenBoard(gridSize);

    // Translate mapText to
    TextToMap();

    // Debugging
    if(DEBUG_MODE)
        DisplayBoard();

}

// Takes the current mapText, and creates the Grid from it
void MapController::TextToMap()
{
    // Retrieve the size of our playboard
    int boardSize = playboard->boardSize;
    // String stream, to record all items in the Grid.
    stringstream sStream;
    int textIndex = 0;

    // Report Progress
    cout << "|==> Converting Map to Matrix..." << endl;

    // Outer Loop for each Row output
    for(int i=gridSize-1; i >= 0; i--)
    {
        for(int j=0; j < gridSize; j++)
        {
            // Determine X and Y
            // Subtracting 1 since Game Grid starts at 0
            int xVal = j;
            int yVal = i;

            if(textIndex < mapText.length())
            {
                char t = mapText.at(textIndex++);
                if(t=='\n')
                {
                    // Create Wall Block
                    playboard->Grid[i][j] = FormBlock('#',xVal,yVal);
                }
                // Create a Block type object from the character
                playboard->Grid[i][j] = FormBlock(t,xVal,yVal);
            }
            else
                // Create Wall Block
                playboard->Grid[i][j] = FormBlock('#',xVal,yVal);

        } // END INNER LOOP
        textIndex++;

    } // END OUTER LOOP

}

Player * MapController::GetPlayer()
{
    return this->player;
}

// Returns the pointer to the block coords specified
Block* MapController::GetBlockAt(int xPos, int yPos)
{
    int i = yPos;
    int j = xPos;

    // Edge case
    if(i < 0 || j < 0 || i > gridSize || j > gridSize)
    {
        return new Block();
    }
    Block * temp = playboard->Grid[i][j];
    return temp;
}
// Overload to hand Grid Locations
Block* MapController::GetBlockAt(GridLoc loc)
{
    int i = loc.y;
    int j = loc.x;

    Block * temp = playboard->Grid[i][j];
    return temp;
}

// Returns a string represention of the arg
string MapController::BlockForm(Block * bl)
{

    // Get the type of block
    BLOCK_TYPE type = bl->type;

    // Send back the correct string representation
    // through a switch statement
    switch(type)
    {

    case EMPTY:
        return " ";
        break;
    case WALL:
        return "#";
        break;
    case PLAYER:
        return "@";
        break;
    case ENEMY:
        return "&";
        break;
    case CHEST:
        return "$";
        break;
    case ARROWS:
        return "%";
        break;
    }
} // END Blockform
Block * MapController::FormBlock(char bl, int xPos, int yPos)
{
    // Send back the correct Block child

    if (bl == '#')
    {
        Wall * w = new Wall();
        w->Init(gridSize,0);
        w->Place(xPos,yPos);
        return w;
    }
    else if (bl == '@')
    {
        Player * p = new Player();
        p->Init(gridSize,6);
        p->Place(xPos,yPos);
        player = p;
        return p;
    }
    else if (bl == '&')
    {
        Enemy * e = new Enemy();
        e->Init(playboard->boardSize,4);
        e->Place(xPos,yPos);
        // Store this enemy in our array for checking later
        enemies[eInd++] = e;
        // Track enemies on board
        enemyCount++;
        enemyMax++;
        return e;
    }
    else if (bl == '$')
    {
        Chest * c = new Chest();
        c->Init(playboard->boardSize,0);
        c->Place(xPos,yPos);
        chest = c;
        return c;
    }
    else if (bl == '%')
    {
        Arrows * a = new Arrows();
        a->Init(playboard->boardSize,0);
        a->Place(xPos,yPos);
        arrows = a;
        return a;
    }
    else
    {
        Block * b = new Block();
        b->Init(playboard->boardSize,0);
        b->Place(xPos,yPos);
        emptyCount++;
        return b;
    }
}

void MapController::InitializeGraphics()
{
    // Setup Textures
    LoadBackgroundImage("images/floor.png");           // Load maze background image
    LoadIntroImage("images/intro.png");
    LoadLevelSelectImage("images/levelselect.png");
    LoadGameOverImage("images/died.png");
    LoadLevelCompleteImage("images/win.png");
    LoadMenuImage("images/menu.png");
    LoadDarknessImage("images/darkness_bigger.png");
    LoadStartImage("images/startscreen.png");
    LoadEndGameImage("images/endgame.png");

}


void MapController::LoadEndGameImage(char* FileName)
{
    endGameTexture = TextureLoader(FileName);
}

void MapController::LoadBackgroundImage(char* FileName)
{
    backgroundTexture = TextureLoader(FileName);
}

void MapController::LoadDarknessImage(char* FileName)
{
    darknessTexture = TextureLoader(FileName);
}

void MapController::LoadStartImage(char* FileName)
{
    startTexture = TextureLoader(FileName);
}


void MapController::LoadGameOverImage(char* FileName)
{
    gameOverTexture = TextureLoader(FileName);
}

void MapController::LoadMenuImage(char* FileName)
{
    menuTexture = TextureLoader(FileName);
}

void MapController::LoadLevelCompleteImage(char* FileName)
{
    levelCompleteTexture = TextureLoader(FileName);
}

void MapController::LoadLevelSelectImage(char* FileName)
{
    levelSelectTexture = TextureLoader(FileName);
}

void MapController::LoadIntroImage(char* FileName)
{
    introTexture = TextureLoader(FileName);
}


void MapController::DrawEndGame()
{
    glPushMatrix();
    // glColor3f(0.0,0.0,0.0);
    glBindTexture(GL_TEXTURE_2D,endGameTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,-0.9f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,-0.9f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,-0.9f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,-0.9f);

    glEnd();
    glPopMatrix();
}

void MapController::DrawStart()
{
    glPushMatrix();
    // glColor3f(0.0,0.0,0.0);
    glBindTexture(GL_TEXTURE_2D,startTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,-0.9f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,-0.9f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,-0.9f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,-0.9f);

    glEnd();
    glPopMatrix();
}


void MapController::DrawDarkness()
{
    // Define Variables
    float xVal = 2.0, yVal = 2.0;
    float xTrans = player->Loc.x;
    float yTrans = player->Loc.y;

    // The level becomes brigther the fewer enemies remain
    if(enemyCount > 0)
    {
        /*
        xVal += (gridSize/(enemyMax/xVal))+enemyCount;
        yVal += (gridSize/(enemyMax/yVal))+enemyCount;
        */
        if(gridSize > 15)
        {
            xVal += (float)(15/enemyMax)/enemyCount;
            yVal += (float)(15/enemyMax)/enemyCount;
        }
        else
        {
            xVal += (float)(gridSize/enemyMax)/enemyCount;
            yVal += (float)(gridSize/enemyMax)/enemyCount;
        }
    }


    glPushMatrix();
    // glColor3f(0.0,0.0,0.0);
    glBindTexture(GL_TEXTURE_2D,darknessTexture);
    // Move the darkness with the player
    glTranslatef(xTrans,yTrans,0.0);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-xVal,yVal,-0.9f);

    glTexCoord2f(1,0);
    glVertex3f(xVal,yVal,-0.9f);

    glTexCoord2f(1,1);
    glVertex3f(xVal,-yVal,-0.9f);

    glTexCoord2f(0,1);
    glVertex3f(-xVal,-yVal,-0.9f);

    glEnd();
    glPopMatrix();
}


void MapController::DrawMenu()
{
    glPushMatrix();
    // glColor3f(0.0,0.0,0.0);
    glBindTexture(GL_TEXTURE_2D,menuTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,-0.9f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,-0.9f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,-0.9f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,-0.9f);

    glEnd();
    glPopMatrix();
}


void MapController::DrawGameOver()
{
    glPushMatrix();
    glColor3f(1.0,0.1,0.2);
    glBindTexture(GL_TEXTURE_2D,gameOverTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,-0.9f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,-0.9f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,-0.9f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,-0.9f);

    glEnd();
    glPopMatrix();
}

void MapController::DrawLevelComplete()
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,levelCompleteTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,0.0f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,0.0f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,0.0f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,0.0f);

    glEnd();
    glPopMatrix();
}

void MapController::DrawLevelSelect()
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,levelSelectTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,0.0f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,0.0f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,0.0f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,0.0f);

    glEnd();
    glPopMatrix();
}

void MapController::DrawIntro()
{
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,introTexture);

    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex3f(-1,1,0.0f);

    glTexCoord2f(1,0);
    glVertex3f(1,1,0.0f);

    glTexCoord2f(1,1);
    glVertex3f(1,-1,0.0f);

    glTexCoord2f(0,1);
    glVertex3f(-1,-1,0.0f);

    glEnd();
    glPopMatrix();
}


Block* MapController::PeekBlock(Block * b, DIRECTION dir)
{
    GridLoc gLoc = b->gLoc;
    if(dir==UP)
    {
        return GetBlockAt(gLoc.x,gLoc.y+1);
    }
    else if(dir==DOWN)
    {
        return GetBlockAt(gLoc.x,gLoc.y-1);
    }
    else if(dir==LEFT)
    {
        return GetBlockAt(gLoc.x-1,gLoc.y);
    }
    else if(dir==RIGHT)
    {
        return GetBlockAt(gLoc.x+1,gLoc.y);
    }
    else if(dir == NONE)
    {
        return b;
    }
    else
    {
        return nullptr;
    }
}



// Command the player to move, and return the destination block before it's swapped
void MapController::MovePlayer(DIRECTION dir)
{
    // Update Player Object
    player->MovePlayer(dir);
    // Return the location that the player would end up in
    Block * dest = PeekBlock(player, dir);
    // Set the Players Destination
    player->gDest = dest->gLoc;

}

// Moves both the player and the enemies into place
void MapController::UpdateAllLocations()
{
    // Update the player first
    UpdateLocation(player,GetBlockAt(player->gDest));

    // Then update all enemies
    // Loop through enemies and command them to move towards the player
    for(int e=0; e < enemyMax; e++)
    {
        if(enemies[e]->alive)
            UpdateLocation(enemies[e],GetBlockAt(enemies[e]->gDest));
    }

}


// Swaps the Block with the destination block.
void MapController::UpdateLocation(Block* bl, Block* dest)
{
    // Swap the block pointers, and their Grid Locations
    SwapGridLocations(bl->gLoc.y,bl->gLoc.x, dest->gLoc.y, dest->gLoc.x);
}

// Swaps the Enemy with the destination block.
void MapController::UpdateLocation(Enemy* bl, Block* dest)
{
    // Swap the block pointers, and their Grid Locations
    SwapGridLocations(bl->gLoc.y,bl->gLoc.x, dest->gLoc.y, dest->gLoc.x);
}


void MapController::SwapGridLocations(int i, int j, int x, int y)
{
    SwapBlocks(playboard->Grid[i][j],playboard->Grid[x][y]);
}

void MapController::SwapGridLocations(GridLoc loc1, GridLoc loc2)
{
    SwapBlocks(playboard->Grid[loc1.y][loc1.x],playboard->Grid[loc2.y][loc2.x]);
}


// Used when swapping blocks in the grid to maintain the (x,y) structure of the map
// Called when moving player or the enemies.
void MapController::SwapBlocks(Block*& b1, Block*& b2)
{
    // Get Grid Locations
    GridLoc g1 = b1->gLoc;
    GridLoc g2 = b2->gLoc;

    // Swap locations
    b1->gLoc = g2;
    b2->gLoc = g1;

    // Swap pointers
    Block * temp = b1;
    b1 = b2;
    b2 = temp;

}


// Test if the Player Movement is valid
// True if collision occurs, false otherwise.
bool MapController::DoesCollide(DIRECTION dir)
{
    // Result
    bool result = false;

    // Test if the move is valid
    result = !ValidMove(player, dir);

    // Caller expects opposite result
    return result;
}

// Checks if a Character (Player or Enemy) could move in the direction provided
bool MapController::ValidMove(Block * b1, DIRECTION dir)
{
    // Get Adjecent block and its location
    Block * dest = PeekBlock(b1,dir);

    // Edge case
    if(dest == nullptr)
        return false;

    // Test if colliding with a wall
    if( dest->type == WALL )
    {
        return false;
    }

    if (b1->type == ENEMY)
    {
        // Get this enemy block
        Enemy * eBl = GetEnemyAt(b1->gLoc);

        // Create Enemy pointer
        Enemy * en;

        // Find if any enemy is moving towards this block
        for(int e=0; e < enemyMax; e++)
        {
            en = enemies[e];
            // If this enemy is alive, and our enemy is trying to move
            // into the same spot as their destination, it's an invalid move.
            if(en != eBl && (en->alive && eBl->alive))
            {
                // Enemy cannot go to the same destination
                if(dest->gLoc == en->gDest )
                    return false;
            }
        }
    }

    return true;
}


// Test is Enemies have gotten to the player
// Return True if player is dead, otherwise false
bool MapController::CheckDeath()
{
    // Locations and Destination
    GridLoc pLoc;
    GridLoc pDest;
    GridLoc eLoc;
    GridLoc eDest;

    // Players Location and their their destination
    pLoc = player->gLoc;
    pDest = player->gDest;

    // Checking Enemies collision
    for(int e=0; e< enemyMax; e++)
    {
        // Setup Enemy location and destination
        eLoc = enemies[e]->gLoc;
        eDest = enemies[e]->gDest;

        // Enemy can only kill the player if they are alive, and on thee same block.
        if(enemies[e]->alive)
        {
            // Both reside in same block, both are destined to the same block,
            //  or the player is destined to step into the enemy
            if(pLoc == eLoc || pDest == eDest || pDest == eLoc)
                return true;
        }
    }
    return false;
}

// Test if the Player has reached the chest, or no more enemies remain
bool MapController::CheckWin()
{
    // All Enemies eliminated, we win!
    if(enemyCount == 0)
    {
        return true;
    }
    else
    {
        // Locations of player and chest
        GridLoc pLoc = player->gLoc;
        GridLoc pDest = player->gDest;
        GridLoc cLoc = chest->gLoc;

        if(pLoc == cLoc || pDest == cLoc)
        {
            return true;
        }
    }
    return false;

}

bool MapController::CheckPickupArrows()
{
    // Locations of player destination and chest
    GridLoc pLoc = player->gDest;
    GridLoc aLoc = arrows->gLoc;


    // Player on the arrows space, and they haven't been collected yet.
    if(pLoc == aLoc && !arrows->collected)
    {
        // Add to player arrows
        player->arrowsLeft++;
        // Make arrows spot now blank
        arrows->collected = true;
        return true;
    }

    return false;
}

void MapController::CheckArrowCollision()
{
    if( player->arrowStatus )
    {
        GridLoc aLoc = player->GetArrowLoc();
        Block * block = GetBlockAt(aLoc.x,aLoc.y);

        if( block->type == ENEMY)
        {
            // Get the Enemy pointer at this location
            Enemy * en = GetEnemyAt(aLoc);

            // Kill this enemy, and destroy the arrow
            if(en != nullptr && en->alive)
            {
                enemyCount--;
                en->alive = false;
                player->arrowStatus = false;
            }
        }

        // Arrow hits a wall, destroy the arrow
        if(block->type == WALL)
        {
            player->arrowStatus = false;
        }
    }
}

Enemy * MapController::GetEnemyAt(GridLoc loc)
{
    Enemy * en = nullptr;
    // If the Block is an enemy, get the pointer to that enemy.
    for(int e=0; e< enemyMax; e++)
    {
        if( enemies[e]->gLoc == loc)
        {
            // Found the enemy
            en = enemies[e];
            break;
        }
    }

    return en;
}


// Commands all the enemies to chase the player
void MapController::AwakenEnemies()
{

    // Loop through enemies and command them to move towards the player
    for(int e=0; e < enemyMax; e++)
    {
        if(DEBUG_MODE)
            cout << endl << "|==> Enemy[" << e << "] is chasing the player from (" << enemies[e]->gLoc.x << "," << enemies[e]->gLoc.y << ")" << endl;
        try
        {
            if(enemies[e]->alive)
                ChasePlayer(enemies[e],player->gDest);
        }
        catch(exception e)
        {

        }

    }


}


/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Shortest Path Implementation
/// This uses the Breadth-First Search algorithm to find the player
void MapController::ChasePlayer(Enemy * en, GridLoc pLoc)
{
    if(DEBUG_MODE)
        cout << "|==> Searching for Player at (" << pLoc.x << "," << pLoc.y << ")" << endl;

    // Get our inner Nodepath
    NodePath nodePath;
    NodePath childNodePath;

    // Used inside the loop
    Block * currentBlock;

    // Get the list of directions to go
    int numMoves = 0;
    // Holds current move to try
    DIRECTION currentMove;
    // Holds Valid moves for traversal
    DIRECTION validMoves[5];

    FindValidMoves(en,validMoves,&numMoves);

    // Number of explored Nodes
    int expCount = 0;
    // Array of nodes explored, with a max of the empty blocks on the map
    GridLoc * explored = new GridLoc[emptyCount];

    // Create the NodePath, with the potential size to hold all empty blocks
    NodePath nPath;
    // Add moves to frontier
    for(int i=0; i< numMoves; i++)
    {
        // Give it a valid direction, and the Block we will reach
        nPath.direction = validMoves[i];
        nPath.block = PeekBlock(en,validMoves[i]);
        frontier.push(nPath);
        // Debugging
        if(DEBUG_MODE)
            cout << "|==> Pushing " << BlockForm(nPath.block) << "(" << nPath.block->gLoc.x << "," << nPath.block->gLoc.y << ") to queue by going " << DirectionToString(nPath.direction) << endl;
    }

    // Add the enemy starting position
    explored[expCount++] = en->gLoc;

    // Continue until no moves can be made.
    while(!frontier.empty())
    {
        nodePath = frontier.front();
        frontier.pop();

        // Debugging
        if(DEBUG_MODE)
            cout << "|==> Popped " << BlockForm(nodePath.block) << "(" << nodePath.block->gLoc.x << "," << nodePath.block->gLoc.y << ") from the frontier" << endl;

        // Check if we reached the Player Location
        if( nodePath.block->gLoc == pLoc )
        {
            // FOUND THE PLAYER
            if(DEBUG_MODE)
                cout << "|==> ***!!!! Found the player !!!!*** " << endl;
            // Get the direction and location needed to progress towards the player
            DIRECTION dirTowardsPlayer = nodePath.direction;
            GridLoc gLocTowardsPlayer = PeekBlock(en,dirTowardsPlayer)->gLoc;
            // Command Enemy to move
            en->MoveEnemy(dirTowardsPlayer);
            // Set the Enemies destination
            en->gDest = gLocTowardsPlayer;
            break;
        }

        // Consider this node explored
        if(!isExplored(explored,nodePath.block->gLoc))
            explored[expCount++] = nodePath.block->gLoc;
        if(DEBUG_MODE)
            cout << "|==> Now explored: " << BlockForm(nodePath.block) << "(" << nodePath.block->gLoc.x << "," << nodePath.block->gLoc.y << ")" << endl;

        // Find valid moves from this Block
        FindValidMoves(nodePath.block,validMoves, &numMoves);

        // Add valid moves from that position
        for(int m=0; m<numMoves; m++)
        {
            // Get next move
            currentMove = validMoves[m];

            // Debugging
            if(DEBUG_MODE)
                cout << "|==> Testing Move: " << DirectionToString(currentMove) << endl;

            currentBlock = PeekBlock(nodePath.block,currentMove);
            // Store this in a new NodePath
            childNodePath.block = currentBlock;
            childNodePath.direction = nodePath.direction;

            if(DEBUG_MODE)
                cout << "|==> Found block: " << BlockForm(currentBlock) << "(" << currentBlock->gLoc.x << "," << currentBlock->gLoc.y << ")" << endl;

            if(!isExplored(explored,currentBlock->gLoc))
            {
                // Push this new direction and node into the queue
                frontier.push(childNodePath);

                if(DEBUG_MODE)
                    cout << "|==> Unexplored Node: " << BlockForm(currentBlock) << "(" << currentBlock->gLoc.x << "," << currentBlock->gLoc.y << ") added to the frontier" << endl;

            } // END Not Explored

        } // END Child Nodes loop

    } // END While still frontier moves

    // Empty out the Frontier
    while(!frontier.empty())
        frontier.pop();

    // Remove old explored set before the next enemy
    for(int e = 0; e< emptyCount; e++)
    {
        explored[e] = GridLoc(0,0);
    }

    return;
}

// Returns true/false depending on if the GridLocation argument is within the Explored array.
bool MapController::isExplored( GridLoc * explored, GridLoc gL)
{
    // Check to make sure this Block hasn't been explored already
    for(int e=0; e<emptyCount; e++)
    {
        if(DEBUG_MODE)
            cout << "|==> Testing against Explored: (" << explored[e].x << "," << explored[e].y << ")" << endl;

        // Are the Grid locations matching?
        if(explored[e] == gL)
        {
            if(DEBUG_MODE)
                cout << "|==> Already Explored: (" << gL.x << "," << gL.y << ")" << endl;
            return true;
        }
    }
    return false;
}

// Used in the Shorted Path Algorithm to find the blocks an enemy can move into.
void MapController::FindValidMoves(Block* bl, DIRECTION* moves, int* found)
{
    // Directions to go
    DIRECTION Moves[] = { UP,DOWN,LEFT,RIGHT,NONE };

    if(DEBUG_MODE)
        cout << "Valid moves from: (" << bl->gLoc.x << "," << bl->gLoc.y << ") ";

    // Find the moves that are valid to take
    int vInd = 0;

    for(int m=0; m < 4; m++)
    {
        // If Move is valid, add it to the Valid Moves
        if(ValidMove(bl,Moves[m]))
        {
            moves[vInd++] = Moves[m];
            if(DEBUG_MODE)
                cout << ", " << DirectionToString(Moves[m]);
        }
    }
    // Pass back the number of moves found, and return the list of moves
    *found = vInd;
    if(DEBUG_MODE)
        cout << endl;
    return;
}

