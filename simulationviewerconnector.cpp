#include "simulationviewerconnector.h"

#include <QDateTime>

const float INIT_POWER = 2;
const int INIT_MANIPULATION_AREA = 2;

SimulationViewerConnector::SimulationViewerConnector(Field* field, SimulationViewer* simViewer) :
    mSimViewer(simViewer),
    mVelocityManipulator(field)
{
    connect(simViewer, &SimulationViewer::mouseRightButtonClicked, this, &SimulationViewerConnector::rightMouseClick);
    connect(simViewer, &SimulationViewer::mouseRightButtonMoved, this, &SimulationViewerConnector::rightMouseMove);
    mVelocityManipulator.setPower(INIT_POWER);
    mVelocityManipulator.setManipulationArea(INIT_MANIPULATION_AREA);
}

void SimulationViewerConnector::rightMouseClick(int simX, int simY, int viewX, int viewY)
{
    this->mLastSimX = simX;
    this->mLastSimY = simY;
    this->mLastViewX = viewX;
    this->mLastViewY = viewY;
    this->mTimer = QDateTime::currentMSecsSinceEpoch();
}

void SimulationViewerConnector::rightMouseMove(int simX, int simY, int viewX, int viewY)
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
