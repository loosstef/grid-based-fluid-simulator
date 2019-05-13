#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QMutex>

class Grid;

class Field : public QObject
{
public:
    Field(const int simWidth, const int simHeight, QObject* parent = nullptr);
    ~Field();
    Grid* getMassGrid() const {return mMass;}
    Grid* getSmokeDensityGrid() const {return mSmokeDensity;}
    Grid* getHorizontalVelocity() const {return mHorizontalVelocity;}
    Grid* getVerticalVelocity() const {return mVerticalVelocity;}
    Grid* getWallsGrid() const {return this->mWalls;}
    Grid* getEnergyGrid() const {return  this->mEnergy;}
    int getWidth() const {return this->simWidth;}
    int getHeight() const {return this->simHeight;}
    void reset();
protected:
    int simWidth;
    int simHeight;
    Grid* mMass;
    Grid* mSmokeDensity;
    Grid* mHorizontalVelocity;
    Grid* mVerticalVelocity;
    Grid* mWalls;
    Grid* mEnergy;
    QMutex baseLock;
};

#endif // FIELD_H
