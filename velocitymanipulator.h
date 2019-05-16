#ifndef VELOCITYMANIPULATOR_H
#define VELOCITYMANIPULATOR_H

#include "field.h"
#include "painttool.h"

class VelocityManipulator
{
public:
    VelocityManipulator(Field* field);
    void setPower(double power);
    void setManipulationArea(int size);
    void move(int posX, int posY, double velX, double velY);
private:
    Field* mField;
    PaintTool mHorizontalManipulator;
    PaintTool mVerticalManipulator;
    double mPower = 1;
};

#endif // VELOCITYMANIPULATOR_H
