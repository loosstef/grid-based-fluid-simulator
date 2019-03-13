#ifndef SIMULATIONVIEWERCONNECTOR_H
#define SIMULATIONVIEWERCONNECTOR_H

#include <QObject>
#include "field.h"
#include "velocitymanipulator.h"
#include "simulationviewer.h"

class SimulationViewerConnector : public QObject
{
    Q_OBJECT
public:
    SimulationViewerConnector(Field* field, SimulationViewer* simViewer);
private:
    VelocityManipulator mVelocityManipulator;
    SimulationViewer* mSimViewer;
    int mLastSimX = 0;
    int mLastSimY = 0;
    int mLastViewX = 0;
    int mLastViewY = 0;
    qint64 mTimer;
public slots:
    void rightMouseClick(int simX, int simY, int viewX, int viewY);
    void rightMouseMove(int simX, int simY, int viewX, int viewY);
};

#endif // SIMULATIONVIEWERCONNECTOR_H
