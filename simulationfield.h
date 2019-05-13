#ifndef SIMULATIONFIELD_H
#define SIMULATIONFIELD_H

#include <QObject>
#include "field.h"

class SimulationFieldController;
enum EdgeCaseMethod {wrap, block};
const float THERMAL_EXPENSION_FACTOR = 0.00347222222;

class SimulationField : public Field
{
    Q_OBJECT
public:
    SimulationField(const int width, const int height, QObject* parent = nullptr);
    void simulateNextStep(const int deltaTime);
    SimulationFieldController *getController();
private:
    bool simulateForwardAdvection(int deltaTime);
    void simulateReverseAdvection(int deltaTime);
    void simulatePressureResult(int deltaTime);
    void diffuse(int deltaTime);
    void makeVelocityVectorsNotPointToWalls();
    void tearDownWalls(int deltaTime);
    float calculateDistance(float x1, float y1, float x2, float y2);
    int calcGradientPoints(int xCoords[4], int yCoords[4], float percentages[4], float x, float y);
    int calcGradientPointsHorVerSplit(int xCoords[], int yCoords[], float percentages[], float x, float y);
    int calcGradientPointsDivideByDistance(int xCoords[], int yCoords[], float percentages[], float x, float y);
    bool outOfBoundX(float x);
    bool outOfBoundY(float y);
    bool testValidity();
    void calculateAndEmitDebugData();
    Grid* mLastMass;
    Grid* mLastSmokeDensity;
    Grid* mLastHorizontalVelocity;
    Grid* mLastVerticalVelocity;
    Grid* mLastEnergy;
    bool mForwardAdvection = true;
    bool mReverseAdvection = true;
    bool mPressure = true;
    bool mDiffuse = true;
    EdgeCaseMethod mEdgeCaseMethod = wrap;

signals:
    void totalMassCalculated(float totalMass);
    void avgTempCalculated(float avgTemp);

public slots:
    void toggleSimulationOfFowardAdvection(bool val) {this->mForwardAdvection = val;}
    void toggleSimulationOfReverseAdvection(bool val) {this->mReverseAdvection = val;}
    void toggleSimulationOfPressure(bool val) {this->mPressure = val;}
    void toggleDiffusion(bool val) {this->mDiffuse = val;}
    void changeEdgeCaseMethod(EdgeCaseMethod edgeCaseMethod) {this->mEdgeCaseMethod = edgeCaseMethod;}
};

#endif // SIMULATIONFIELD_H
