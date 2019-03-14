#ifndef VELOCITYMANIPULATOR_H
#define VELOCITYMANIPULATOR_H

#include "field.h"
#include "painttool.h"

class VelocityManipulator
{
public:
    VelocityManipulator(Field* field);
    void setPower(float power);
    void setManipulationArea(int size);
    void move(int posX, int posY, float velX, float velY);
private:
    Field* mField;
    PaintTool mHorizontalManipulator;
    PaintTool mVerticalManipulator;
    float mPower = 1;
};

#endif // VELOCITYMANIPULATOR_H
