#include "simulationfield.h"

#include <cstdlib>

SimulationField::SimulationField(int width, int height) :
    Field(width, height)
{

}

/**
 * Change the field so it represents the next step in the simulation,
 * while taking the elapsed time into account
 * @brief SimulationField::simulateNextStep
 * @param deltaTime
 * @return true if everything went well
 */
// TODO: fill in this function
bool SimulationField::simulateNextStep(int deltaTime)
{
    /*
    for(int x = 0; x < this->width; ++x) {
        for (int y = 0; y < this->height; ++y) {
            this->density.set(x, y, (float)(rand()%255));
        }
    }*/
    return true;
}
