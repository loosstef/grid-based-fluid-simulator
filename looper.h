#ifndef LOOPER_H
#define LOOPER_H
#include <QThread>
#include <QPixmap>
#include <QImage>
#include <QMutex>
#include "simulationviewer.h"
#include "field.h"
#include "painttool.h"
#include "renderengine.h"

class SimulationField;


class Looper : public QThread
{
    Q_OBJECT
    void run() override;
public:
    Looper(SimulationField* simField, RenderEngine* renderEngine, QObject* parent = nullptr);
signals:
    void FieldUpdated(QImage* image);
private:
    SimulationField* mSimField;
    RenderEngine* mRenderEngine;
    bool running = true;
    int sleepPerLoop = 0;
    QMutex runLock;
public slots:
    void toggleRunning();
    void setSleepPerLoop(int value) {this->sleepPerLoop = value;}
};

#endif // LOOPER_H
