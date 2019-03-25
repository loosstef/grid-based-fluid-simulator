#ifndef FIELD_H
#define FIELD_H

#include "grid.h"
#include <QPixmap>
#include <QObject>
#include <QMutex>

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
    void reset();
protected:
    int simWidth;
    int simHeight;
    Grid* mDensity;
    Grid* mSmokeDensity;
    Grid* mHorizontalVelocity;
    Grid* mVerticalVelocity;
    QMutex baseLock;
};

#endif // FIELD_H
