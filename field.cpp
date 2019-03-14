#include "field.h"
#include <QImage>

#include <cstdlib>
#include <QtMath>

Field::Field(int simWidth, int simHeight) :
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
