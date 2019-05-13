#include "field.h"

#include "grid.h"

const int INIT_ENERGY = 288;

Field::Field(const int simWidth, const int simHeight, QObject* parent) :
    QObject(parent),
    simWidth(simWidth), simHeight(simHeight)
{
    this->mMass = new Grid(simWidth, simHeight, 1);
    this->mSmokeDensity = new Grid(simWidth, simHeight);
    this->mHorizontalVelocity = new Grid(simWidth, simHeight);
    this->mVerticalVelocity = new Grid(simWidth, simHeight);
    this->mWalls = new Grid(simWidth, simHeight);
    this->mEnergy = new Grid(simWidth, simHeight, 273+15);
}

Field::~Field()
{
    delete this->mMass;
    delete this->mSmokeDensity;
    delete this->mHorizontalVelocity;
    delete this->mVerticalVelocity;
    delete this->mWalls;
    delete this->mEnergy;
}

/**
 * Reset the whole field, start again from nothing
 * @brief Field::reset
 */
void Field::reset()
{
    this->baseLock.lock();
    this->mMass->reset(1);
    this->mSmokeDensity->reset(0);
    this->mHorizontalVelocity->reset(0);
    this->mVerticalVelocity->reset(0);
    this->mEnergy->reset(INIT_ENERGY);
    this->baseLock.unlock();
}
