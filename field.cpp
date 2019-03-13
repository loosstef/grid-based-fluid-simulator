#include "field.h"
#include <QImage>

#include <cstdlib>
#include <QtMath>

Field::Field(int simWidth, int simHeight) :
    simWidth(simWidth), simHeight(simHeight),
    mDensity(simWidth, simHeight, 1), mSmokeDensity(simWidth, simHeight),
    mHorizontalVelocity(simWidth, simHeight), mVerticalVelocity(simWidth, simHeight)
{

}
