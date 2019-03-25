#ifndef SIMULATIONFIELDCONTROLLER_H
#define SIMULATIONFIELDCONTROLLER_H

#include <QObject>
#include "field.h"
#include "velocitymanipulator.h"
#include "simulationviewer.h"

class SimulationField;

class SimulationFieldController : public QObject
{
    Q_OBJECT
public:
    SimulationFieldController(SimulationField* field);
    void connectToView(SimulationViewer* simViewer);
private:
    VelocityManipulator mVelocityManipulator;
    int mLastSimX = 0;
    int mLastSimY = 0;
    int mLastViewX = 0;
    int mLastViewY = 0;
    qint64 mTimer;
public slots:
    void rightMouseClick(int simX, int simY, int viewX, int viewY);
    void rightMouseMove(int simX, int simY, int viewX, int viewY);
};

#endif // SIMULATIONFIELDCONTROLLER_H
