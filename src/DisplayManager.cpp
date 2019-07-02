#include "DisplayManager.h"

using namespace std;

// Constructor
DisplayManager::DisplayManager()
{

}

// Destructor
DisplayManager::~DisplayManager()
{

}

// Sets the MapController for display
void DisplayManager::LinkMap(MapController* mC)
{
    this->mapCtrl = mC;
}

void DisplayManager::Display()
{

    // Retrieve the size of our playboard
    PlayBoard * pb = mapCtrl->GetBoard();
    int boardSize = pb->boardSize;
    // Clear previous output
    system("cls");

    // Outer Loop for each Row output
    for(int i=0; i < boardSize; i++)
    {

        for(int j=0; j < boardSize; j++)
        {

            Block * temp = pb->Grid[i][j];

            if (temp == nullptr)
            {
                std::cout << "X";
            }

            if(j == boardSize-1)
            {
                std::cout << mapCtrl->BlockForm(temp) << endl;
            }
            else
            {
                std::cout << mapCtrl->BlockForm(temp);
            }


        } // END INNER LOOP


    } // END OUTER LOOP



}

