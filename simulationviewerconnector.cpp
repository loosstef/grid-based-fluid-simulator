#include "simulationviewerconnector.h"

#include <QDateTime>

SimulationViewerConnector::SimulationViewerConnector(Field* field, SimulationViewer* simViewer) :
    mSimViewer(simViewer),
    mVelocityManipulator(field)
{
    /*
    connect(simViewer, &SimulationViewer::mouseRightButtonClicked, this, &SimulationViewerConnector::rightMouseClick);
    connect(simViewer, &SimulationViewer::mouseRightButtonMoved, this, &SimulationViewerConnector::rightMouseMove);
    */
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
    int horizontalSpeed = (viewX - this->mLastViewX) / deltaTime;
    int verticalSpeed = (viewY - this->mLastViewY) / deltaTime;
    this->mVelocityManipulator.move(this->mLastSimX, this->mLastSimY, horizontalSpeed, verticalSpeed);
    // Reset all variables for the next iteration
    this->mLastSimX = simX;
    this->mLastSimY = simY;
    this->mLastViewX = viewX;
    this->mLastViewY = viewY;
    this->mTimer = QDateTime::currentMSecsSinceEpoch();
}
