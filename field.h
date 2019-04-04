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
    Grid* getDensityGrid() const {return mDensity;}
    Grid* getSmokeDensityGrid() const {return mSmokeDensity;}
    Grid* getHorizontalVelocity() const {return mHorizontalVelocity;}
    Grid* getVerticalVelocity() const {return mVerticalVelocity;}
    int getWidth() const {return this->simWidth;}
    int getHeight() const {return this->simHeight;}
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
