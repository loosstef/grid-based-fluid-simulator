#ifndef SIMULATIONFIELDCONTROLLER_H
#define SIMULATIONFIELDCONTROLLER_H

#include <QObject>
#include "velocitymanipulator.h"

class QComboBox;
class QCheckBox;
class QPushButton;
class SimulationField;
class SimulationViewer;

class SimulationFieldController : public QObject
{
    Q_OBJECT
public:
    SimulationFieldController(SimulationField* field, QObject* parent = nullptr);
    void connectToViewer(SimulationViewer* simViewer);
    void connectToSimSettingsCheckboxes(QCheckBox* forwardAdvectionToggle, QCheckBox* reverseAdvectionToggle, QCheckBox* pressureToggle, QCheckBox *diffusionToggle);
    void connectToResetButton(QPushButton* resetBtn);
    void connectEdgeCaseSelector(QComboBox* edgeCaseSelector);
private:
    VelocityManipulator mVelocityManipulator;
    SimulationField* mSimField;
    int mLastSimX = 0;
    int mLastSimY = 0;
    int mLastViewX = 0;
    int mLastViewY = 0;
    qint64 mTimer;
public slots:
    void rightMouseClick(int simX, int simY, int viewX, int viewY);
    void rightMouseMove(int simX, int simY, int viewX, int viewY);
    void edgeCaseChanged(int index);
};

#endif // SIMULATIONFIELDCONTROLLER_H
