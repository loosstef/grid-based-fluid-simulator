#ifndef LOOPER_H
#define LOOPER_H
#include <QThread>
#include "simulationfield.h"
#include "simulationviewer.h"
#include "field.h"
#include <QPixmap>
#include <QImage>
#include "painttool.h"


class Looper : public QThread
{
    Q_OBJECT
    void run() override;
public:
    Looper(SimulationField* simField, int width, int height, QObject* parent = nullptr);
signals:
    void FieldUpdated(QImage* image);
private:
    SimulationField* mSimField;
    int mWidth;
    int mHeight;
};

#endif // LOOPER_H
