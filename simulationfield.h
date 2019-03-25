#ifndef SIMULATIONFIELD_H
#define SIMULATIONFIELD_H

#include <QObject>
#include <QMutex>
#include "field.h"
#include "simulationfieldcontroller.h"

enum EdgeCaseMethod {reflect, wrap, block};

class SimulationField : public Field
{
    Q_OBJECT
public:
    SimulationField(int width, int height, QObject* parent = nullptr);
    bool simulateNextStep(int deltaTime);
    SimulationFieldController *getController();
private:
    bool simulateForwardAdvection(int deltaTime);
    void simulateReverseAdvection(int deltaTime);
    void simulatePressureResult(int deltaTime);
    float calculateDistance(float x1, float y1, float x2, float y2);
    int calcGradientPoints(int xCoords[4], int yCoords[4], float percentages[4], float x, float y);
    int calcGradientPointsHorVerSplit(int xCoords[], int yCoords[], float percentages[], float x, float y);
    int calcGradientPointsDivideByDistance(int xCoords[], int yCoords[], float percentages[], float x, float y);
    bool testValidity();
    Grid* mLastDensity;
    Grid* mLastSmokeDensity;
    Grid* mLastHorizontalVelocity;
    Grid* mLastVerticalVelocity;
    bool mForwardAdvection = true;
    bool mReverseAdvection = true;
    bool mPressure = true;
    EdgeCaseMethod mEdgeCaseMethod = reflect;

public slots:
    void toggleSimulationOfFowardAdvection(bool val) {this->mForwardAdvection = val;}
    void toggleSimulationOfReverseAdvection(bool val) {this->mReverseAdvection = val;}
    void toggleSimulationOfPressure(bool val) {this->mPressure = val;}
    void changeEdgeCaseMethod(EdgeCaseMethod edgeCaseMethod) {this->mEdgeCaseMethod = edgeCaseMethod;}
};

#endif // SIMULATIONFIELD_H
