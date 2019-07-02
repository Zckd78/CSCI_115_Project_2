/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 * Extended by Zachary Scott
 * for CSCI 115 - Project 02
 */

#include <string.h>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <windows.h>
#include <math.h>

#include <Maze.h>

/* GLUT callback Handlers */

using namespace std;

Maze * M;

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2,0,(GLsizei) height,(GLsizei) height);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    // Sleep to save CPU cycles, and smooth the gameplay
    Sleep(5);
    // Have the Map Controller Display everything
    M->Display();

    glutSwapBuffers();
}

void key(unsigned char key, int x, int y)
{
    // Used to make sure we don't register
    // one keyPress through multiple states
    bool onePress = false;


    if(M->state == INTRO)
    {
        // Space was pressed during the intro, move to Level Select
        if( key == ' ' )
        {
            M->state = SELECT;
        }
    }
    // Keybindings during a level
    if(M->state == SELECT)
    {
        switch (key)
        {
        case '1':
            M->StartLevel(1);
            M->state = LEVEL;
            break;
        case '2':
            M->StartLevel(2);
            M->state = LEVEL;
            break;
        case '3':
            M->StartLevel(3);
            M->state = LEVEL;
            break;
        case '4':
            M->StartLevel(4);
            M->state = LEVEL;
            break;
        }
    }

    if (!onePress && M->state == MENU)
    {
        switch (key)
        {
        case 27 :
            M->state = LEVEL;
            onePress = true;
            break;
        case 'r':
            M->state = LEVEL;
            M->StartLevel(M->currentLevel);
            break;
        case 'q':
            exit(0);
            break;
        }
    }

    // Keybindings during a level
    if(!onePress && M->state == LEVEL)
    {
        switch (key)
        {

            case 'r':
            // Dev Only, reloads the level
            if(MAP_TESTING_MODE)
            {
                M->StartLevel(M->currentLevel);
            }
            break;

        case 'w':
            M->TryMovePlayer(UP);
            break;

        case 's':
            M->TryMovePlayer(DOWN);
            break;

        case 'a':
            M->TryMovePlayer(LEFT);
            break;

        case 'd':
            // cout<< M->GetPlayer()->GetPlayerLoc().x<< "    "<<M->GetPlayer()->GetPlayerLoc().y<<endl;
            M->TryMovePlayer(RIGHT);
            break;

        case ' ':
            // if(!M->liveSetOfArrws)      // if setof arrows were picked by player
            M->TryShootArrow();
            break;
        // Pressing Esc to get the menu
        case 27 :
            M->state = MENU;
            onePress = true;
            break;
        }
    }




    // Can only restart when you lose
    if (M->state == LOSE)
    {
        switch (key)
        {
        case 'r':
            M->StartLevel(M->currentLevel);
            M->state = LEVEL;
            break;
        }
    }

    if (M->state == WIN)
    {
        switch (key)
        {
        case ' ':

            // Game only has 4 levels, proceed to ENDGAME
            if (M->currentLevel == 4)
            {
                M->state = ENDGAME;
            }
            // Otherwise go to next level
            else {
                M->currentLevel++;
                M->StartLevel(M->currentLevel);
                M->state = LEVEL;
            }
            break;
        }
    }

    glutPostRedisplay();
}


void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

void idle(void)
{

    //Your Code here

    glutPostRedisplay();
}


void mouse(int btn, int state, int x, int y)
{

    switch(btn)
    {
    case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN)
        {

            GetOGLPos(x,y);
        }
        break;


    case GLUT_RIGHT_BUTTON:

        if(state==GLUT_DOWN)
        {

            GetOGLPos(x,y);
        }
        break;
    }
    glutPostRedisplay();
};

void Specialkeys(int key, int x, int y)
{

    // Only have the player act during a level
    if(M->state == LEVEL)
    {
        switch(key)
        {
        case GLUT_KEY_UP:
            M->TurnPlayer(UP);
            break;

        case GLUT_KEY_DOWN:
            M->TurnPlayer(DOWN);
            break;

        case GLUT_KEY_LEFT:
            M->TurnPlayer(LEFT);
            break;

        case GLUT_KEY_RIGHT:
            // cout<< M->GetPlayer()->GetPlayerLoc().x<< "    "<<M->GetPlayer()->GetPlayerLoc().y<<endl;
            M->TurnPlayer(RIGHT);
            break;
        }
    }

    glutPostRedisplay();
}


void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create the Maze class, which directs most actions.
    cout << "|==> Creating Maze..." << endl;
    M = new Maze();

}


/* Program entry point */

int main(int argc, char *argv[])
{
    cout << "|==> Starting main" << endl;
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (600, 600);                //window screen
    glutInitWindowPosition (400, 0);            //window position
    glutCreateWindow ("Maze");                    //program title
    cout << "|==> Initializing Maze" << endl;

    init();

    glutDisplayFunc(display);                     //callback function for display
    glutReshapeFunc(resize);                      //callback for reshape
    glutKeyboardFunc(key);                        //callback function for keyboard
    glutSpecialFunc(Specialkeys);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    cout << "|==> Starting the OpenGL Loop" << endl;
    glutMainLoop();

    cout << "|==> Deleting Maze..." << endl;
    delete M;

    return EXIT_SUCCESS;
}
