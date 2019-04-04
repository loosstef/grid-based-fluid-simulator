#include "field.h"

#include "grid.h"

Field::Field(const int simWidth, const int simHeight, QObject* parent) :
    QObject(parent),
    simWidth(simWidth), simHeight(simHeight)
{
    this->mDensity = new Grid(simWidth, simHeight, 1);
    this->mSmokeDensity = new Grid(simWidth, simHeight);
    this->mHorizontalVelocity = new Grid(simWidth, simHeight);
    this->mVerticalVelocity = new Grid(simWidth, simHeight);
}

Field::~Field()
{
    delete this->mDensity;
    delete this->mSmokeDensity;
    delete this->mHorizontalVelocity;
    delete this->mVerticalVelocity;
}

/**
 * Reset the whole field, start again from nothing
 * @brief Field::reset
 */
void Field::reset()
{
    this->baseLock.lock();
    this->mDensity->reset(1);
    this->mSmokeDensity->reset(0);
    this->mHorizontalVelocity->reset(0);
    this->mVerticalVelocity->reset(0);
    this->baseLock.unlock();
}
