#include "velocitymanipulator.h"

#include "grid.h"

VelocityManipulator::VelocityManipulator(Field* field) :
    mField(field),
    mHorizontalManipulator(field->getHorizontalVelocity()),
    mVerticalManipulator(field->getVerticalVelocity())
{

}

/**
 * set the power the user has when adding velocity to the scene
 * @brief VelocityManipulator::setPower
 * @param power higher when the change in speed goes up when stroking
 */
void VelocityManipulator::setPower(float power)
{
    this->mPower = power;
}

/**
 * Set the size of the area affected when you brush the scene
 * @brief VelocityManipulator::setManipulationArea
 * @param size
 */
void VelocityManipulator::setManipulationArea(int size)
{
    this->mHorizontalManipulator.setSize(size);
    this->mVerticalManipulator.setSize(size);
}

/**
 * Add movement of fluid in a certain point, with a certain velocity
 * @brief VelocityManipulator::move
 * @param posX x-coordinate of the base of the movement
 * @param posY y-coordinate of the base of the movement
 * @param velX x-coordinate of the movement vector
 * @param velY y-coordinate of the movement vector
 */
void VelocityManipulator::move(int posX, int posY, float velX, float velY)
{
    this->mHorizontalManipulator.setHardness(velX*this->mPower);
    this->mVerticalManipulator.setHardness(velY*this->mPower);
    this->mHorizontalManipulator.drawPoint(posX, posY);
    this->mVerticalManipulator.drawPoint(posX, posY);
}
