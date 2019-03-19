#ifndef SIMULATIONFIELD_H
#define SIMULATIONFIELD_H

#include "field.h"
#include "simulationfieldcontroller.h"


class SimulationField : public Field
{
public:
    SimulationField(int width, int height);
    bool simulateNextStep(int deltaTime);
    SimulationFieldController *getController();
private:
    bool simulateForwardAdvection(int deltaTime);
    float calculateDistance(float x1, float y1, float x2, float y2);
    int calcGradientPoints(int xCoords[4], int yCoords[4], float percentages[4], float x, float y);
    int calcGradientPointsHorVerSplit(int xCoords[], int yCoords[], float percentages[], float x, float y);
    int calcGradientPointsDivideByDistance(int xCoords[], int yCoords[], float percentages[], float x, float y);
    Grid* mLastDensity;
    Grid* mLastSmokeDensity;
    Grid* mLastHorizontalVelocity;
    Grid* mLastVerticalVelocity;
};

#endif // SIMULATIONFIELD_H
