#ifndef FIELD_H
#define FIELD_H

#include "grid.h"
#include <QPixmap>
#include <QObject>

class Field : public QObject
{
public:
    Field(int simWidth, int simHeight, QObject* parent = nullptr);
    ~Field();
    Grid* getDensityGrid() {return mDensity;}
    Grid* getSmokeDensityGrid() {return mSmokeDensity;}
    Grid* getHorizontalVelocity() {return mHorizontalVelocity;}
    Grid* getVerticalVelocity() {return mVerticalVelocity;}
    int getWidth() {return this->simWidth;}
    int getHeight() {return this->simHeight;}
protected:
    int simWidth;
    int simHeight;
    Grid* mDensity;
    Grid* mSmokeDensity;
    Grid* mHorizontalVelocity;
    Grid* mVerticalVelocity;
};

#endif // FIELD_H
