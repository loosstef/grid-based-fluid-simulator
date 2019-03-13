#ifndef FIELD_H
#define FIELD_H

#include "grid.h"
#include <QPixmap>

class Field
{
public:
    Field(int simWidth, int simHeight);
    void render(QImage* image);
    Grid* getDensityGrid() {return &density;}
    int getWidth() {return this->simWidth;}
    int getHeight() {return this->simHeight;}
protected:
    int simWidth;
    int simHeight;
    Grid density;
    Grid horizontalVelocity;
    Grid verticalVelocity;
private:
    float valueToColorIntensity(float value);
};

#endif // FIELD_H
