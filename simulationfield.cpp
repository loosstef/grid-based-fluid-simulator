#include "simulationfield.h"
#include <QtMath>
#include <math.h>
#include "simulationfieldcontroller.h"

const double SLOWNESS_FORWARD_ADVECTION = 100;
const double SLOWNESS_REVERSE_ADVECTION = 100;
const double PRESSURE_SLOWNESS = 50;
const int DIFFUSE_SLOWNESS = 200;
const int WALL_DIFFUSE_SLOWNESS = 600;
const int METHOD_OF_DIVISION = 1;
const double MAX_MOVEMENT_VECTOR_SIZE = 0.8;
const double WALL_TEAR_DOWN_TEMP = 323;

/**
 * Constructor. Generates a new simulation-field with a certain
 * width and height.
 * @brief SimulationField::SimulationField
 * @param width the width of the field
 * @param height the height of the field
 * @param parent
 */
SimulationField::SimulationField(const int width, const int height, QObject* parent) :
    Field(width, height, parent)
{

}

/**
 * Change the field so it represents the next step in the simulation,
 * while taking the elapsed time into account
 * @brief SimulationField::simulateNextStep
 * @param deltaTime
 */
void SimulationField::simulateNextStep(const int deltaTime)
{
    this->baseLock.lock();
    if(this->mForwardAdvection) {
        this->mLastMass = Grid::deepCopy(mMass);
        this->mLastSmokeDensity = Grid::deepCopy(mSmokeDensity);
        this->mLastHorizontalVelocity = Grid::deepCopy(mHorizontalVelocity);
        this->mLastVerticalVelocity = Grid::deepCopy(mVerticalVelocity);
        this->mLastEnergy = Grid::deepCopy(mEnergy);

        this->simulateForwardAdvection(deltaTime);

        delete this->mLastMass;
        delete this->mLastSmokeDensity;
        delete this->mLastHorizontalVelocity;
        delete this->mLastVerticalVelocity;
        delete this->mLastEnergy;
    }
    if(this->mReverseAdvection) {
        this->mLastMass = Grid::deepCopy(mMass);
        this->mLastSmokeDensity = Grid::deepCopy(mSmokeDensity);
        this->mLastHorizontalVelocity = Grid::deepCopy(mHorizontalVelocity);
        this->mLastVerticalVelocity = Grid::deepCopy(mVerticalVelocity);
        this->mLastEnergy = Grid::deepCopy(mEnergy);

        this->simulateReverseAdvection(deltaTime);

        delete mLastMass;
        delete mLastSmokeDensity;
        delete mLastHorizontalVelocity;
        delete mLastVerticalVelocity;
        delete mLastEnergy;
    }
    if(this->mPressure) {
        this->simulatePressureResult(deltaTime);
    }

    // remove outward pointing vectors when edge-block-mode is on
    if(this->mEdgeCaseMethod == block) {
        for(int x = 0; x < this->simWidth; ++x) {
            if(this->mVerticalVelocity->get(x, 0) < 0) {
                this->mVerticalVelocity->set(x, 0, 0);
            }
            if(this->mVerticalVelocity->get(x, this->simHeight-1) > 0) {
                this->mVerticalVelocity->set(x, this->simHeight-1, 0);
            }
        }
        for(int y = 0; y < this->simHeight; ++y) {
            if(this->mHorizontalVelocity->get(0, y) < 0) {
                this->mHorizontalVelocity->set(0, y, 0);
            }
            if(this->mHorizontalVelocity->get(this->simWidth-1, y) > 0) {
                this->mHorizontalVelocity->set(this->simWidth-1, y, 0);
            }
        }
    }

    if(this->mDiffuse) {
        this->mLastMass = Grid::deepCopy(mMass);
        this->mLastHorizontalVelocity = Grid::deepCopy(mHorizontalVelocity);
        this->mLastVerticalVelocity = Grid::deepCopy(mVerticalVelocity);
        this->mLastEnergy = Grid::deepCopy(mEnergy);

        this->diffuse(deltaTime);

        delete this->mLastMass;
        delete this->mLastHorizontalVelocity;
        delete this->mLastVerticalVelocity;
        delete this->mLastEnergy;
    }

    this->tearDownWalls(deltaTime);

    // change velocities based on walls
    this->mLastMass = Grid::deepCopy(mMass);
    this->mLastSmokeDensity = Grid::deepCopy(mSmokeDensity);
    this->mLastHorizontalVelocity = Grid::deepCopy(mHorizontalVelocity);
    this->mLastVerticalVelocity = Grid::deepCopy(mVerticalVelocity);

    this->makeVelocityVectorsNotPointToWalls();

    delete mLastMass;
    delete mLastSmokeDensity;
    delete mLastHorizontalVelocity;
    delete mLastVerticalVelocity;

    this->testValidity();
    this->calculateAndEmitDebugData();
    this->baseLock.unlock();
}

/**
 * Returns a new controller to control this simulationField
 * @brief SimulationField::getController
 * @return
 */
SimulationFieldController *SimulationField::getController()
{
    return new SimulationFieldController(this);
}

/**
 * Do a part of the simulation
 * the forward advection
 * @brief SimulationField::simulateForwardAdvection
 * @param deltaTime the time-interval to the next step we have to calcutlate
 * @return true if everything went wel
 */
bool SimulationField::simulateForwardAdvection(int deltaTime)
{
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            bool currPosIsWall = this->mWalls->get(x, y) > 0;
            if(currPosIsWall) {
                continue;
            }
            // init data
            double sourceMass = this->mLastMass->get(x, y);
            double sourceSmokeDensity = this->mLastSmokeDensity->get(x, y);
            double sourceHorVel = this->mLastHorizontalVelocity->get(x, y);
            double sourceVerVel = this->mLastVerticalVelocity->get(x, y);
            double sourceHorMovement = (sourceHorVel*deltaTime/SLOWNESS_FORWARD_ADVECTION);
            double sourceVerMovement = (sourceVerVel*deltaTime/SLOWNESS_FORWARD_ADVECTION);
            double sourceEnergy = this->mLastEnergy->get(x, y);
            if(qAbs(sourceHorMovement) > MAX_MOVEMENT_VECTOR_SIZE) {
                sourceHorMovement = ((sourceHorMovement > 0) ? 1 : -1) * MAX_MOVEMENT_VECTOR_SIZE;
            }
            if(qAbs(sourceVerMovement) > MAX_MOVEMENT_VECTOR_SIZE) {
                sourceVerMovement = ((sourceVerMovement > 0) ? 1 : -1) * MAX_MOVEMENT_VECTOR_SIZE;
            }
            Q_ASSERT(sourceMass >= 0);
            Q_ASSERT(sourceSmokeDensity >= 0);
            int targetX[4];
            int targetY[4];
            double targetPercentage[4];
            int nTargets = this->calcGradientPoints(targetX, targetY, targetPercentage, x+sourceHorMovement, y+sourceVerMovement);

            if(nTargets == 0) {
                this->mHorizontalVelocity->set(x, y, 0);
                this->mVerticalVelocity->set(x, y, 0);
                continue;
            }
            for(int i = 0; i < nTargets; ++i) {
                double massValue = sourceMass * targetPercentage[i];
                Q_ASSERT(massValue >= 0);
                double smokeDensityValue = sourceSmokeDensity * targetPercentage[i];
                double VelXValue = sourceHorVel * targetPercentage[i];
                double VelYValue = sourceVerVel * targetPercentage[i];
                double energyValue = sourceEnergy * targetPercentage[i];
                this->mMass->add(x, y, -massValue);
                this->mMass->add(targetX[i], targetY[i], massValue);
                this->mSmokeDensity->add(x, y, -smokeDensityValue);
                this->mSmokeDensity->add(targetX[i], targetY[i], smokeDensityValue);
                this->mHorizontalVelocity->add(x, y, -VelXValue);
                this->mHorizontalVelocity->add(targetX[i], targetY[i], VelXValue);
                this->mVerticalVelocity->add(x, y, -VelYValue);
                this->mVerticalVelocity->add(targetX[i], targetY[i], VelYValue);
                this->mEnergy->add(x, y, -energyValue);
                this->mEnergy->add(targetX[i], targetY[i], energyValue);

                // FIXME: there should be a better solution than this
                Q_ASSERT(this->mMass->get(x, y) >= -0.0001);
                Q_ASSERT(this->mSmokeDensity->get(x, y) >= -0.0001);
                if(this->mMass->get(x, y) < 0) {
                    mMass->set(x, y, 0);
                }
                if(this->mSmokeDensity->get(x, y) < 0) {
                    mSmokeDensity->set(x, y, 0);
                }
            }
        }
    }
}

/**
 * Do a part of the simulation
 * the reverse advection
 * @brief SimulationField::simulateReverseAdvection
 * @param deltaTime
 */
void SimulationField::simulateReverseAdvection(int deltaTime)
{
    int* nSources = new int[this->simWidth * this->simHeight];
    int (*sourceX)[4] = new int[this->simWidth * this->simHeight][4];
    int (*sourceY)[4] = new int[this->simWidth * this->simHeight ][4];
    double (*sourcePercentage)[4] = new double[this->simWidth * this->simHeight][4];
    double* totalAskedPercentages = new double[this->simWidth * this->simHeight];

    for(int i = 0; i < this->simWidth * this->simHeight; ++i) {
        totalAskedPercentages[i] = 0;
    }

    // init all arrays
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            // init data
            double sourceHorVel = this->mLastHorizontalVelocity->get(x, y);
            double sourceVerVel = this->mLastVerticalVelocity->get(x, y);
            double sourceHorMovement = (sourceHorVel*deltaTime/SLOWNESS_FORWARD_ADVECTION);
            double sourceVerMovement = (sourceVerVel*deltaTime/SLOWNESS_FORWARD_ADVECTION);
            if(qAbs(sourceHorMovement) > MAX_MOVEMENT_VECTOR_SIZE) {
                sourceHorMovement = ((sourceHorMovement > 0) ? 1 : -1) * MAX_MOVEMENT_VECTOR_SIZE;
            }
            if(qAbs(sourceVerMovement) > MAX_MOVEMENT_VECTOR_SIZE) {
                sourceVerMovement = ((sourceVerMovement > 0) ? 1 : -1) * MAX_MOVEMENT_VECTOR_SIZE;
            }
            nSources[x+y*this->simWidth] = this->calcGradientPoints(
                sourceX[x+y*this->simWidth],
                sourceY[x+y*this->simWidth],
                sourcePercentage[x+y*this->simWidth],
                x-sourceHorMovement,
                y-sourceVerMovement
            );
            for(int i = 0; i < nSources[x+y*this->simWidth]; ++i) {
                int xCoord = sourceX[x+y*this->simWidth][i];
                int yCoord = sourceY[x+y*this->simWidth][i];
                double percentageAsked = sourcePercentage[x+y*this->simWidth][i];
                totalAskedPercentages[xCoord+yCoord*this->simWidth] += percentageAsked;
            }
        }
    }

    //move everything
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            if(this->mWalls->get(x, y) > 0) {
                continue;
            }
            for(int i = 0; i < nSources[x+y*this->simWidth]; ++i) {
                int sX = sourceX[x+y*this->simWidth][i];
                int sY = sourceY[x+y*this->simWidth][i];
                double totalSourcePercentage = totalAskedPercentages[sX+sY*this->simWidth];
                double askedPercentage = sourcePercentage[x+y*this->simWidth][i];
                if(totalSourcePercentage > 1) {
                    askedPercentage /= totalSourcePercentage;
                }
                double massValue = this->mLastMass->get(sX, sY) * askedPercentage;
                double smokeDensityValue = this->mLastSmokeDensity->get(sX, sY) * askedPercentage;
                double horVelValue = this->mLastHorizontalVelocity->get(sX, sY) * askedPercentage;
                double verVelValue = this->mLastVerticalVelocity->get(sX, sY) * askedPercentage;
                double energyValue = this->mLastEnergy->get(sX, sY) * askedPercentage;
                Q_ASSERT(!isinf(smokeDensityValue));
                Q_ASSERT(smokeDensityValue >= 0);
                this->mMass->add(sX, sY, -massValue);
                this->mMass->add(x, y, massValue);
                this->mSmokeDensity->add(sX, sY, -smokeDensityValue);
                this->mSmokeDensity->add(x, y, smokeDensityValue);
                Q_ASSERT(!isinf(this->mSmokeDensity->get(x, y)));
                Q_ASSERT(this->mSmokeDensity->get(sX, sY) > -0.001);
                // FIXME: there should be a better solution
                if(this->mSmokeDensity->get(sX, sY) < 0) {
                    this->mSmokeDensity->set(sX, sY, 0);
                }
                this->mHorizontalVelocity->add(sX, sY, -horVelValue);
                this->mHorizontalVelocity->add(x, y, horVelValue);
                this->mVerticalVelocity->add(sX, sY, -verVelValue);
                this->mVerticalVelocity->add(x, y, verVelValue);
                this->mEnergy->add(sX, sY, -energyValue);
                this->mEnergy->add(x, y, energyValue);
            }
        }
    }

    delete[] nSources;
    delete[] sourceX;
    delete[] sourceY;
    delete[] sourcePercentage;
    delete[] totalAskedPercentages;
}

/**
 * Add velocities to the field so points with higher density will move
 * to points with a lower density.
 * @brief SimulationField::simulatePressureResult
 * @param deltaTime
 */
void SimulationField::simulatePressureResult(int deltaTime)
{
    for(int y = 0; y < this->simHeight; ++y) {
        for(int x = 0; x < this->simWidth; ++x) {
            if(this->mWalls->get(x, y) > 0) {
                continue;
            }
            double localPressure = this->mEnergy->get(x, y) * THERMAL_EXPENSION_FACTOR;
            double forceX = 0;
            double forceY = 0;
            if(this->mEdgeCaseMethod == block) {
                if(x + 1 < this->simWidth && this->mWalls->get(x+1, y) <= 0) {
                    double remotePressure = this->mEnergy->get(x+1, y) * THERMAL_EXPENSION_FACTOR;
                    forceX = localPressure - remotePressure;
                }
                if(y + 1 < this->simHeight && this->mWalls->get(x, y+1) <= 0) {
                    double remotePressure = this->mEnergy->get(x, y+1) * THERMAL_EXPENSION_FACTOR;
                    forceY = localPressure - remotePressure;
                }
                double velX = forceX * deltaTime / PRESSURE_SLOWNESS;
                double velY = forceY * deltaTime / PRESSURE_SLOWNESS;
                this->mHorizontalVelocity->add(x, y, velX);
                this->mHorizontalVelocity->add(x+1, y, velX);
                this->mVerticalVelocity->add(x, y, velY);
                this->mVerticalVelocity->add(x, y+1, velY);
            }
            else if(this->mEdgeCaseMethod == wrap) {
                int nextX = (x + 1) % this->simWidth;
                int nextY = (y + 1) % this->simHeight;
                if(this->mWalls->get(nextX, y) <= 0) {
                    double remotePressure = this->mEnergy->get(nextX, y) * THERMAL_EXPENSION_FACTOR;
                    forceX = localPressure - remotePressure;
                }
                if(this->mWalls->get(x, nextY) <= 0) {
                    double remotePressure = this->mEnergy->get(x, nextY) * THERMAL_EXPENSION_FACTOR;
                    forceY = localPressure - remotePressure;
                }
                double velX = forceX * deltaTime / PRESSURE_SLOWNESS;
                double velY = forceY * deltaTime / PRESSURE_SLOWNESS;
                this->mHorizontalVelocity->add(x, y, velX);
                this->mHorizontalVelocity->add(nextX, y, velX);
                this->mVerticalVelocity->add(x, y, velY);
                this->mVerticalVelocity->add(x, nextY, velY);
            }
        }
    }
}

/**
 * Diffuse all the values in the field
 * @brief SimulationField::diffuse
 * @param deltaTime
 */
void SimulationField::diffuse(int deltaTime)
{
    // TODO: make use of deltaTime?
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simWidth; ++y) {
            if(this->mWalls->get(x, y) > 0) {
                continue;
            }
            int nSurroundingGridPoints = 0;
            double massSum = 0;
            double horVelSum = 0;
            double verVelSum = 0;
            double energySum = 0;
            for(int surrX = x-1; surrX <= x+1; ++surrX) {
                for(int surrY = y-1; surrY <= y+1; ++surrY) {
                    if(this->mWalls->get(surrX, surrY) > 0) {
                        continue;
                    }
                    if(this->mWalls->get(surrX, y) > 0 && this->mWalls->get(x, surrY) > 0) {
                        continue;
                    }
                    if(this->mEdgeCaseMethod == block) {
                        if(surrX >= 0 && surrX < this->simWidth && surrY >=0 && surrY < this->simHeight) {
                            ++nSurroundingGridPoints;
                            massSum += this->mLastMass->get(surrX, surrY);
                            horVelSum += this->mLastHorizontalVelocity->get(surrX, surrY);
                            verVelSum += this->mLastVerticalVelocity->get(surrX, surrY);
                            energySum += this->mLastEnergy->get(surrX, surrY);
                        }
                    } else if(this->mEdgeCaseMethod == wrap) {
                        int realSurrX = (surrX+this->simWidth)%this->simWidth;
                        int realSurrY = (surrY+this->simHeight)%this->simHeight;
                        ++nSurroundingGridPoints;
                        massSum += this->mLastMass->get(realSurrX, realSurrY);
                        horVelSum += this->mLastHorizontalVelocity->get(realSurrX, realSurrY);
                        verVelSum += this->mLastVerticalVelocity->get(realSurrX, realSurrY);
                        energySum += this->mLastEnergy->get(realSurrX, realSurrY);
                    }
                }
            }
            // TODO: check if the speed-problem exists here
            int ownWeight = DIFFUSE_SLOWNESS - nSurroundingGridPoints;
            massSum += this->mLastMass->get(x, y) * ownWeight;
            horVelSum += this->mLastHorizontalVelocity->get(x, y) * ownWeight;
            verVelSum += this->mLastVerticalVelocity->get(x, y) * ownWeight;
            energySum += this->mLastEnergy->get(x, y) * ownWeight;
            this->mMass->set(x, y, massSum/DIFFUSE_SLOWNESS);
            this->mHorizontalVelocity->set(x, y, horVelSum/DIFFUSE_SLOWNESS);
            this->mVerticalVelocity->set(x, y, verVelSum/DIFFUSE_SLOWNESS);
            this->mEnergy->set(x, y, energySum/DIFFUSE_SLOWNESS);
        }
    }
}

void SimulationField::makeVelocityVectorsNotPointToWalls()
{
    for(int x = 0; x < this->getWidth(); ++x) {
        for(int y = 0; y < this->getHeight(); ++y) {
            // reset speed, smoke and temperature of walls
            if(this->mWalls->get(x, y) > 0) {
                this->mVerticalVelocity->set(x, y, 0);
                this->mHorizontalVelocity->set(x, y, 0);
                this->mSmokeDensity->set(x, y, 0);
                this->mEnergy->set(x, y, mMass->get(x, y) * INIT_ENERGY);
            }
            // upper wall
            int upperY = y - 1;
            if(this->mEdgeCaseMethod == wrap && upperY < 0) {
                upperY = this->getHeight() - 1;
            }
            if(this->mWalls->get(x, upperY) > 0 && this->mVerticalVelocity->get(x, y) < 0) {
                mVerticalVelocity->set(x, y, 0);
            }
            // lower wall
            int lowerY = y + 1;
            if(this->mEdgeCaseMethod == wrap && lowerY >= this->getHeight()) {
                lowerY = 0;
            }
            if(this->mWalls->get(x, lowerY) > 0 && this->mVerticalVelocity->get(x, y) > 0) {
                this->mVerticalVelocity->set(x, y, 0);
            }
            // left wall
            int leftX = x - 1;
            if(this->mEdgeCaseMethod == wrap && leftX < 0) {
                leftX = this->getWidth() - 1;
            }
            if(this->mWalls->get(leftX, y) > 0 && this->mHorizontalVelocity->get(x, y) < 0) {
                this->mHorizontalVelocity->set(x, y, 0);
            }
            // right wall
            int rightX = x + 1;
            if(this->mEdgeCaseMethod == wrap && rightX >= this->getWidth()) {
                rightX = 0;
            }
            if(this->mWalls->get(rightX, y) > 0 && this->mHorizontalVelocity->get(x, y) > 0) {
                this->mHorizontalVelocity->set(x, y, 0);
            }
        }
    }

}

void SimulationField::tearDownWalls(int deltaTime)
{
    for(int x = 0; x < this->getWidth(); ++x) {
        for(int y = 0; y < this->getHeight(); ++y) {
            if(this->mWalls->get(x, y) > 0) {
                bool hotNeighbor = false;
                for(int surrX = x-1; surrX <= x+1; ++surrX) {
                    for(int surrY = y-1; surrY <= y+1; ++surrY) {
                        int realSurrX = surrX;
                        int realSurrY = surrY;
                        if(mEdgeCaseMethod == wrap) {
                            realSurrX = (surrX+this->simWidth)%this->simWidth;
                            realSurrY = (surrY+this->simHeight)%this->simHeight;
                        }
                        double temperature = mEnergy->get(realSurrX, realSurrY) / mMass->get(realSurrX, realSurrY);
                        if(temperature >= WALL_TEAR_DOWN_TEMP) {
                            hotNeighbor = true;
                        }
                    }
                }
                if(hotNeighbor) {
                    mWalls->add(x, y, -((double)deltaTime)/1000.0f);
                }
            }
        }
    }
}

/**
 * Calculate the distance between 2 points
 * @brief SimulationField::calculateDistance
 * @param x1 x-coordinate of the first point
 * @param y1 y-coordinate of the first point
 * @param x2 x-coordinate of the second point
 * @param y2 y-coordinate of the second point
 * @return the distance between the 2 points
 */
double SimulationField::calculateDistance(double x1, double y1, double x2, double y2)
{
    return qSqrt(qPow(x2 - x1, 2) + qPow(y2 - y1, 2));
}

/**
 * Calculate the distribution of a point along the surrounding grid points
 * @brief SimulationField::calcGradientPoints
 * @param xCoords an array of min. size 4 with the x-coordinates of the affected grid points
 * @param yCoords an array of min. size 4 with the y-coordinates of the affected grid points
 * @param percentages an array of min. size 4 with the distribution-percentages of each affected grid-point
 * @param x the x-coordinate of the point wich has to be distributed
 * @param y the y-coordinate of the point wich has to be distributed
 * @return the length of the filled in arrays
 */
int SimulationField::calcGradientPoints(int xCoords[4], int yCoords[4], double percentages[4], double x, double y)
{
    // init the percentages array
    for(int i = 0; i < 4; ++i) {
        percentages[i] = 0;
    }

    // TODO: add assertion for checking if one of the four points is the starting point of the velocity vector

    int nPoints;

    if(METHOD_OF_DIVISION == 1) {
        nPoints = this->calcGradientPointsHorVerSplit(xCoords, yCoords, percentages, x, y);
    } else if(METHOD_OF_DIVISION == 2) {
        nPoints = this->calcGradientPointsDivideByDistance(xCoords, yCoords, percentages, x, y);
    } else {
        return 0;
    }

    double sum = 0;
    for(int i = 0; i < nPoints; ++i) {
        sum += percentages[i];
    }
    Q_ASSERT(sum <= 1.000001);
    if(sum > 1) {
        for(int i = 0; i < nPoints; ++i) {
            percentages[i] /= 1.000001;
        }
    }

    return nPoints;
}

int SimulationField::calcGradientPointsHorVerSplit(int xCoords[], int yCoords[], double percentages[], double x, double y)
{
    int leftMostCoord = qFloor(x);
    int rightMostCoord = leftMostCoord + 1;
    int upperMostCoord = qFloor(y);
    int lowerMostCoord = upperMostCoord + 1;

    // Point A is the upper Left grid-point
    // Point B is the upper right grid-point
    // Point C is the lower left grid-point
    // Point D is the lower right grid-point
    double percentageAB = 0;
    double percentageCD = 0;
    int index = 0;

    if(this->mEdgeCaseMethod == block) {
        if(lowerMostCoord < 0 || upperMostCoord >= this->simHeight) {
            return 0;
        }
        if(rightMostCoord < 0 || leftMostCoord >= this->simWidth) {
            return 0;
        }
    }

    if(upperMostCoord < 0 && this->mEdgeCaseMethod != wrap) {
        percentageCD = 1;
    } else if (lowerMostCoord >= this->simHeight && this->mEdgeCaseMethod != wrap) {
        percentageAB = 1;
    } else {
        percentageAB = 1 - (y - upperMostCoord);
        percentageCD = y - upperMostCoord;
    }

    if(percentageAB != 0) {
        if(leftMostCoord < 0 && this->mEdgeCaseMethod != wrap && this->mWalls->get(rightMostCoord, upperMostCoord) <= 0) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB;
            ++index;
        } else if (rightMostCoord >= this->simWidth && this->mEdgeCaseMethod != wrap && this->mWalls->get(leftMostCoord, upperMostCoord) <= 0) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB;
            ++index;
        } else {
            int wrappedLeftMostCoord = (leftMostCoord + this->simWidth)%this->simWidth;
            int wrappedUpperMostCoord = (upperMostCoord + this->simHeight)%this->simHeight;
            if(this->mWalls->get(wrappedLeftMostCoord, wrappedUpperMostCoord) <= 0) {
                xCoords[index] = wrappedLeftMostCoord;
                yCoords[index] = wrappedUpperMostCoord;
                percentages[index] = percentageAB * (1 - (x - leftMostCoord));
                ++index;
            }
            int wrappedRightMostCoord = (rightMostCoord + this->simWidth)%this->simWidth;
            if(this->mWalls->get(wrappedRightMostCoord, wrappedUpperMostCoord) <= 0) {
                xCoords[index] = wrappedRightMostCoord;
                yCoords[index] = wrappedUpperMostCoord;
                percentages[index] = percentageAB * (x - leftMostCoord);
                ++index;
            }
        }
    }

    if(percentageCD != 0) {
        if(leftMostCoord < 0 && this->mEdgeCaseMethod != wrap && this->mWalls->get(rightMostCoord, lowerMostCoord) <= 0) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD;
            ++index;
        } else if (rightMostCoord >= this->simWidth && this->mEdgeCaseMethod != wrap && this->mWalls->get(leftMostCoord, lowerMostCoord) <= 0) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD;
            ++index;
        } else {
            int wrappedLeftMostCoord = (leftMostCoord + this->simWidth)%this->simWidth;
            int wrappedLowerMostCoord = (lowerMostCoord + this->simHeight)%this->simHeight;
            if(this->mWalls->get(wrappedLeftMostCoord, wrappedLowerMostCoord) <= 0) {
                xCoords[index] = wrappedLeftMostCoord;
                yCoords[index] = wrappedLowerMostCoord;
                percentages[index] = percentageCD * (1 - (x - leftMostCoord));
                ++index;
            }
            int wrappedRightMostCoord = (rightMostCoord + this->simWidth)%this->simWidth;
            if(this->mWalls->get(wrappedRightMostCoord, wrappedLowerMostCoord) <= 0) {
                xCoords[index] = wrappedRightMostCoord;
                yCoords[index] = (lowerMostCoord + this->simHeight)%this->simHeight;
                percentages[index] = percentageCD * (x - leftMostCoord);
                ++index;
            }
        }
    }

    return index;
}

int SimulationField::calcGradientPointsDivideByDistance(int xCoords[], int yCoords[], double percentages[], double x, double y)
{
    // Calculate the surrounding coordinates
    int leftMostCoord = qFloor(x);
    int rightMostCoord = leftMostCoord + 1;
    int upperMostCoord = qFloor(y);
    int lowerMostCoord = upperMostCoord + 1;
    int index = 0;
    if(upperMostCoord >= 0) {
        if(leftMostCoord >= 0 ) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = upperMostCoord;
            ++index;
        }
        if(rightMostCoord < this->simWidth) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = upperMostCoord;
            ++index;
        }
    }
    if(lowerMostCoord < this->simHeight) {
        if(leftMostCoord >= 0) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = lowerMostCoord;
            ++index;
        }
        if(rightMostCoord < this->simWidth) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = lowerMostCoord;
            ++index;
        }
    }
    int nSurroundingPoints = index;

    // Check if the given point overlays a grid-point (+- 0.00001)
    for(int i = 0; i < nSurroundingPoints; ++i) {
        bool overlays = this->calculateDistance(x, y, xCoords[i], yCoords[i]) < 0.00001;
        if(overlays) {
            percentages[i] = 1;
            return nSurroundingPoints;
        }
    }

    // If not, calculate their weights and total weight
    double weights[4];
    double totalWeight = 0;
    for(int i = 0; i < nSurroundingPoints; ++i) {
        double weight = 1 / (qPow(this->calculateDistance(x, y, xCoords[i], yCoords[i]), 1));
        weights[i] = weight;
        totalWeight += weight;
    }

    // Calculate percentages
    for(int i = 0; i < nSurroundingPoints; ++i) {
        percentages[i] = weights[i] / totalWeight;
        if (percentages[i] != percentages[i]) {
            printf("Error");
        }
    }

    return nSurroundingPoints;
}

bool SimulationField::outOfBoundX(double x)
{
    return x < 0 || x > this->simWidth - 1;
}

bool SimulationField::outOfBoundY(double y)
{
    return y < 0 || y > this->simHeight - 1;
}

/**
 * Do a lot of tests on the field to check if everything is still right
 * @brief SimulationField::testValidity
 * @return
 */
bool SimulationField::testValidity()
{
    // Check for sum of densities
    double sumOfDensities = 0;
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            sumOfDensities += this->mMass->get(x, y);
        }
    }
    emit totalMassCalculated(sumOfDensities);
    Q_ASSERT(qAbs(sumOfDensities - this->simWidth*this->simHeight) < 10);
}

void SimulationField::calculateAndEmitDebugData()
{
    double weightedSumOfTemperatures = 0;
    for(int x = 0; x < this->simWidth; ++x) {
        for(int y = 0; y < this->simHeight; ++y) {
            weightedSumOfTemperatures += mEnergy->get(x, y);
        }
    }
    emit avgTempCalculated(weightedSumOfTemperatures / (this->simWidth*this->simHeight));
}
