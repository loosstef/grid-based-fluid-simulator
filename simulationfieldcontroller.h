#ifndef SIMULATIONFIELDCONTROLLER_H
#define SIMULATIONFIELDCONTROLLER_H

#include <QObject>
#include "velocitymanipulator.h"

class QComboBox;
class QCheckBox;
class QPushButton;
class SimulationField;

class SimulationFieldController : public QObject
{
    Q_OBJECT
public:
    SimulationFieldController(SimulationField* field, QObject* parent = nullptr);
    void connectToSimSettingsCheckboxes(QCheckBox* forwardAdvectionToggle, QCheckBox* reverseAdvectionToggle, QCheckBox* pressureToggle, QCheckBox *diffusionToggle);
    void connectToResetButton(QPushButton* resetBtn);
    void connectEdgeCaseSelector(QComboBox* edgeCaseSelector);
private:
    SimulationField* mSimField;
public slots:
    void edgeCaseChanged(int index);
};

#endif // SIMULATIONFIELDCONTROLLER_H
