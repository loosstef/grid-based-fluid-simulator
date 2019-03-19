#ifndef LOOPER_H
#define LOOPER_H
#include <QThread>
#include "simulationfield.h"
#include "simulationviewer.h"
#include "field.h"
#include <QPixmap>
#include <QImage>
#include "painttool.h"
#include "renderengine.h"


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
};

#endif // LOOPER_H
