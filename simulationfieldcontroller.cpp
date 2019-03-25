#include "simulationfieldcontroller.h"

#include <QDateTime>
#include "simulationfield.h"

const float INIT_POWER = 10;
const int INIT_MANIPULATION_AREA = 1;

SimulationFieldController::SimulationFieldController(SimulationField* field, QObject* parent) :
    QObject(parent),
    mVelocityManipulator((Field*)field),
    mSimField(field)
{
    mVelocityManipulator.setPower(INIT_POWER);
    mVelocityManipulator.setManipulationArea(INIT_MANIPULATION_AREA);
}

/**
 * Make the connections between the simulationviewer and the field
 * @brief SimulationFieldController::connectToView
 * @param simViewer
 */
void SimulationFieldController::connectToViewer(SimulationViewer *simViewer)
{
    connect(simViewer, &SimulationViewer::mouseRightButtonClicked, this, &SimulationFieldController::rightMouseClick);
    connect(simViewer, &SimulationViewer::mouseRightButtonMoved, this, &SimulationFieldController::rightMouseMove);
}

void SimulationFieldController::connectToSimSettingsCheckboxes(QCheckBox *forwardAdvectionToggle, QCheckBox *reverseAdvectionToggle, QCheckBox *pressureToggle)
{
    connect(forwardAdvectionToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfFowardAdvection);
    connect(reverseAdvectionToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfReverseAdvection);
    connect(pressureToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfPressure);
}

void SimulationFieldController::connectToResetButton(QPushButton *resetBtn)
{
    connect(resetBtn, &QPushButton::released, mSimField, &SimulationField::reset);
}

void SimulationFieldController::connectEdgeCaseSelector(QComboBox *edgeCaseSelector)
{
    connect(edgeCaseSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &SimulationFieldController::edgeCaseChanged);
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

void SimulationFieldController::edgeCaseChanged(int index)
{
    EdgeCaseMethod method = (EdgeCaseMethod) index;
    this->mSimField->changeEdgeCaseMethod(method);
}
