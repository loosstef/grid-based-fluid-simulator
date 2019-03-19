#include "simulationfieldcontroller.h"

#include <QDateTime>

const float INIT_POWER = 6;
const int INIT_MANIPULATION_AREA = 2;

SimulationFieldController::SimulationFieldController(Field* field) :
    mVelocityManipulator(field)
{
    mVelocityManipulator.setPower(INIT_POWER);
    mVelocityManipulator.setManipulationArea(INIT_MANIPULATION_AREA);
}

/**
 * Make the connections between the simulationviewer and the field
 * @brief SimulationFieldController::connectToView
 * @param simViewer
 */
void SimulationFieldController::connectToView(SimulationViewer *simViewer)
{
    connect(simViewer, &SimulationViewer::mouseRightButtonClicked, this, &SimulationFieldController::rightMouseClick);
    connect(simViewer, &SimulationViewer::mouseRightButtonMoved, this, &SimulationFieldController::rightMouseMove);
}

void SimulationFieldController::rightMouseClick(int simX, int simY, int viewX, int viewY)
{
    this->mLastSimX = simX;
    this->mLastSimY = simY;
    this->mLastViewX = viewX;
    this->mLastViewY = viewY;
    this->mTimer = QDateTime::currentMSecsSinceEpoch();
}

void SimulationFieldController::rightMouseMove(int simX, int simY, int viewX, int viewY)
{
    qint64 deltaTime = QDateTime::currentMSecsSinceEpoch() - this->mTimer;
    float horizontalSpeed = (float)(viewX - this->mLastViewX) / (float)deltaTime;
    float verticalSpeed = (float)(viewY - this->mLastViewY) / (float)deltaTime;
    this->mVelocityManipulator.move(this->mLastSimX, this->mLastSimY, horizontalSpeed, verticalSpeed);
    // Reset all variables for the next iteration
    this->mLastSimX = simX;
    this->mLastSimY = simY;
    this->mLastViewX = viewX;
    this->mLastViewY = viewY;
    this->mTimer = QDateTime::currentMSecsSinceEpoch();
}
