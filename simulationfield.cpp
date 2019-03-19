#include "simulationfield.h"

#include <cstdlib>
#include <QtMath>

const float SLOWNESS = 100;

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
    // TODO: take deltaTime into account
    // init all new-grids
    this->mLastDensity = new Grid(this->mDensity);
    this->mLastSmokeDensity = new Grid(this->mSmokeDensity);
    this->mLastHorizontalVelocity = new Grid(this->mHorizontalVelocity);
    this->mLastVerticalVelocity = new Grid(this->mVerticalVelocity);

    simulateForwardAdvection(deltaTime);

    delete mLastDensity;
    delete mLastSmokeDensity;
    delete mLastHorizontalVelocity;
    delete mLastVerticalVelocity;
    return true;
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
            // init data
            float sourceDensity = this->mLastDensity->get(x, y);
            float sourceSmokeDensity = this->mLastSmokeDensity->get(x, y);
            float sourceHorVel = this->mLastHorizontalVelocity->get(x, y);
            float sourceVerVel = this->mLastVerticalVelocity->get(x, y);
            // TODO: remove debug code
            if(sourceDensity < 0) {
                printf("Error");
            }
            int targetX[4];
            int targetY[4];
            float targetPercentage[4];
            int nTargets = this->calcGradientPoints(targetX, targetY, targetPercentage, x+(sourceHorVel*deltaTime/SLOWNESS), y+(sourceVerVel*deltaTime/SLOWNESS));

            if(nTargets == 0) {
                this->mDensity->set(x, y, 0);
                this->mSmokeDensity->set(x, y, 0);
                this->mHorizontalVelocity->set(x, y, 0);
                this->mVerticalVelocity->set(x, y, 0);
                continue;
            }
            for(int i = 0; i < nTargets; ++i) {
                float densityValue = sourceDensity * targetPercentage[i];
                // TODO: remove debug code
                if(densityValue < 0) {
                    printf("Debug error");
                }
                float smokeDensityValue = sourceSmokeDensity * targetPercentage[i];
                float VelXValue = sourceHorVel * targetPercentage[i];
                float VelYValue = sourceVerVel * targetPercentage[i];
                this->mDensity->add(x, y, -densityValue);
                // TODO: remove debug code
                if(this->mDensity->get(x, y) < 0) {
                    printf("Debug Error");
                }
                this->mDensity->add(targetX[i], targetY[i], densityValue);
                this->mSmokeDensity->add(x, y, -smokeDensityValue);
                this->mSmokeDensity->add(targetX[i], targetY[i], smokeDensityValue);
                this->mHorizontalVelocity->add(x, y, -VelXValue);
                this->mHorizontalVelocity->add(targetX[i], targetY[i], VelXValue);
                this->mVerticalVelocity->add(x, y, -VelYValue);
                this->mVerticalVelocity->add(targetX[i], targetY[i], VelYValue);
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
float SimulationField::calculateDistance(float x1, float y1, float x2, float y2)
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
int SimulationField::calcGradientPoints(int xCoords[4], int yCoords[4], float percentages[4], float x, float y)
{
    // init the percentages array
    for(int i = 0; i < 4; ++i) {
        percentages[i] = 0;
    }

    int leftMostCoord = qFloor(x);
    int rightMostCoord = leftMostCoord + 1;
    int upperMostCoord = qFloor(y);
    int lowerMostCoord = upperMostCoord + 1;

    // Point A is the upper Left grid-point
    // Point B is the upper right grid-point
    // Point C is the lower left grid-point
    // Point D is the lower right grid-point
    float percentageAB = 0;
    float percentageCD = 0;
    int index = 0;

    if(lowerMostCoord < 0 || upperMostCoord >= this->simHeight) {
        return 0;
    }
    if(rightMostCoord < 0 || leftMostCoord >= this->simWidth) {
        return 0;
    }

    if(upperMostCoord < 0) {
        percentageCD = 1;
    } else if (lowerMostCoord >= this->simHeight) {
        percentageAB = 1;
    } else {
        percentageAB = 1 - (y - upperMostCoord);
        percentageCD = y - upperMostCoord;
    }

    if(percentageAB != 0) {
        if(leftMostCoord < 0) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB;
            ++index;
        } else if (rightMostCoord >= this->simWidth) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB;
            ++index;
        } else {
            xCoords[index] = leftMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB * (1 - (x - leftMostCoord));
            ++index;
            xCoords[index] = rightMostCoord;
            yCoords[index] = upperMostCoord;
            percentages[index] = percentageAB * (x - leftMostCoord);
            ++index;
        }
    }

    if(percentageCD != 0) {
        if(leftMostCoord < 0) {
            xCoords[index] = rightMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD;
            ++index;
        } else if (rightMostCoord >= this->simWidth) {
            xCoords[index] = leftMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD;
            ++index;
        } else {
            xCoords[index] = leftMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD * (1 - (x - leftMostCoord));
            ++index;
            xCoords[index] = rightMostCoord;
            yCoords[index] = lowerMostCoord;
            percentages[index] = percentageCD * (x - leftMostCoord);
            ++index;
        }
    }

    // FIXME: this shouldn't be nescesary
    // TODO: delete debug code
    float sum = 0;
    for(int i = 0; i < index; ++i) {
        sum += percentages[i];
        if(percentages[i] < 0) {
            printf("Debug error");
        }
    }
    if(sum >= 1) {
        for(int i = 0; i < index; ++i) {
            percentages[i] = (float)percentages[i] / (float)1.001;
        }
    }
    if(sum > 1) {
        printf("Error");
    }
    if(index == 0) {
        printf("Debug warning");
    }
    if(index == 2) {
        //printf("Debug stop");
    }

    return index;

    /*
    // Calculate the surrounding coordinates
    int leftMostCoord = (int) x;
    int rightMostCoord = leftMostCoord + 1;
    int upperMostCoord = (int) y;
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
    float weights[4];
    float totalWeight = 0;
    for(int i = 0; i < nSurroundingPoints; ++i) {
        float weight = 1 / (qPow(this->calculateDistance(x, y, xCoords[i], yCoords[i]), 1));
        weights[i] = weight;
        totalWeight += weight;
    }*/

    // Calculate percentages
    /*
    for(int i = 0; i < nSurroundingPoints; ++i) {
        percentages[i] = weights[i] / totalWeight;
        if (percentages[i] != percentages[i]) {
            printf("Error");
        }
    }*/

    /*
    //TODO: remove debug code
    if(nSurroundingPoints == 0) {
        printf("Debug Error");
    }
    //TODO: remove debug code
    float sumOfPercentages = 0;
    for(int i = 0; i < nSurroundingPoints; ++i) {
        sumOfPercentages += percentages[i];
    }
    if(qAbs(sumOfPercentages - 1) > 0.01) {
        printf("Debug Error");
    }

    return nSurroundingPoints;*/
}
