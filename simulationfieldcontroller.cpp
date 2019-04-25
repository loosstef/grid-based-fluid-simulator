#include "simulationfieldcontroller.h"
#include <QDateTime>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include "simulationfield.h"
#include "simulationviewer.h"

SimulationFieldController::SimulationFieldController(SimulationField* field, QObject* parent) :
    QObject(parent),
    mSimField(field)
{

}

void SimulationFieldController::connectToSimSettingsCheckboxes(
        QCheckBox *forwardAdvectionToggle,
        QCheckBox *reverseAdvectionToggle,
        QCheckBox *pressureToggle,
        QCheckBox *diffusionToggle
        )
{
    connect(forwardAdvectionToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfFowardAdvection);
    connect(reverseAdvectionToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfReverseAdvection);
    connect(pressureToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleSimulationOfPressure);
    connect(diffusionToggle, &QCheckBox::toggled, mSimField, &SimulationField::toggleDiffusion);
}

void SimulationFieldController::connectToResetButton(QPushButton *resetBtn)
{
    connect(resetBtn, &QPushButton::released, mSimField, &SimulationField::reset);
}

void SimulationFieldController::connectEdgeCaseSelector(QComboBox *edgeCaseSelector)
{
    connect(edgeCaseSelector, qOverload<int>(&QComboBox::currentIndexChanged), this, &SimulationFieldController::edgeCaseChanged);
}

void SimulationFieldController::edgeCaseChanged(int index)
{
    EdgeCaseMethod method = (EdgeCaseMethod) index;
    this->mSimField->changeEdgeCaseMethod(method);
}
